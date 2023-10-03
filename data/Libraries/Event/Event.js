var source = null;
var eventArr = [];
var connected = false;
var eventType = null;
var connectCb = null;
var disconnectCb = null;

function isFunction(functionToCheck) {
    return functionToCheck && {}.toString.call(functionToCheck) === '[object Function]';
}

function debounce(func, wait) {
    var timeout;
    var waitFunc;

    return function () {
        if (isFunction(wait)) {
            waitFunc = wait;
        }
        else {
            waitFunc = function () { return wait };
        }

        var context = this, args = arguments;
        var later = function () {
            timeout = null;
            func.apply(context, args);
        };
        clearTimeout(timeout);
        timeout = setTimeout(later, waitFunc());
    };
}

// reconnectFrequencySeconds doubles every retry
var reconnectFrequencySeconds = 1;

var reconnectFunc = debounce(function () {
    init(eventType);
    // Double every attempt to avoid overwhelming server
    reconnectFrequencySeconds *= 2;
    // Max out at ~1 minute as a compromise between user experience and server load
    if (reconnectFrequencySeconds >= 64) {
        reconnectFrequencySeconds = 64;
    }
}, function () { return reconnectFrequencySeconds * 1000 });

var init = function (event) {
    if ( !window.EventSource ) {
        console.log("[Events] - Events are not supported");
        return;
    }

    eventType = event;
    source = new EventSource(event);

    source.addEventListener('open', function (e) {
        reconnectFrequencySeconds = 1;
        connected = true;
        console.log(`[EVENTS] - Connected to "${event}"`);
        emitConnected();
    }, false);

    source.addEventListener('error', function (e) {
        if (e.target.readyState != EventSource.OPEN) {
            connected = false;
            console.log(`[EVENTS] - Can't connect to "${event}"`);
            source.close();
            reconnectFunc();
            emitDisconnected();
        }
    }, false);

    addHandlers();
};

var emitConnected = function(){
    if( connectCb ){ connectCb(); }
}

var emitDisconnected = function(){
    if( disconnectCb ){ disconnectCb(); }
}

/*
*   Returns if source is connected.
*/
var isConnected = function () {
    return connected;
}

/*
*   Adding handlers that are registered before initialization.
*/
var addHandlers = function () {
    for (const evt of eventArr) {
        source.addEventListener(evt.type, evt.cb, false);
    }
}

/*
*   Removes a handler callback if exists.
*/
var removeHandler = function (evtType) {
    let indexToRemove = -1;
    for (const evt of eventArr) {
        if (evt.type == evtType) {
            indexToRemove = eventArr.indexOf(evt);
            break;
        }
    }
    if (indexToRemove !== -1) {
        source.removeEventListener(eventArr[indexToRemove].type, eventArr[indexToRemove].cb);
        eventArr.splice(indexToRemove, 1);
    }
}

/*
*   Registering an event callback
*/
var on = function (evtType, cb) {
    if( evtType === "connect" ){
        connectCb = cb;
        return;
    }else if( evtType === "disconnect" ){
        disconnectCb = cb;
        return;
    }

    if (source) {
        source.addEventListener(evtType, cb, false);
    } else {
        let evtDesc = {
            type: evtType,
            cb: cb
        };
        eventArr.push(evtDesc);
    }
};

export { init, on, removeHandler, isConnected }