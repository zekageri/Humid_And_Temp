import * as Event from "/Libraries/Event/Event.js";

Event.init("/humidAndTime");

var tempData = [];
var humidityData = [];
var timeData = [];

var options = {
    series: [
        {
            name: "Temperature",
            type: "line",
            data: tempData
        },
        {
            name: "Humidity",
            type: "line",
            data: humidityData
        }
    ],
    chart: {
        id: 'realtime',
        height: 350,
        type: 'line',
        animations: {
            enabled: true,
            easing: 'linear',
            dynamicAnimation: {
                speed: 1000
            }
        },
        toolbar: {
            show: true
        },
        zoom: {
            enabled: true
        }
    },
    dataLabels: {
        enabled: false
    },
    stroke: {
        curve: 'smooth'
    },
    title: {
        text: 'Temperature & Humidity',
        align: 'left'
    },
    markers: {
        size: 0
    },
    xaxis: {
        labels: {
            datetimeUTC: false
        },
        type: 'datetime',
        categories: timeData
    },
    yaxis: [
        {
            seriesName: "Temperature",
            min:-15,
            max: 50,
            axisTicks: {
                show: true
            },
            axisBorder: {
                show: true,
                color: "#008FFB"
            },
            labels: {
                style: {
                    colors: ["#008FFB"]
                }
            },
            title: {
                text: "Temp",
                style: {
                    color: "#008FFB"
                }
            },
            tooltip: {
                enabled: true
            }
        },
        {
            seriesName: "Humidity",
            opposite: true,
            min: 0,
            max: 100,
            axisTicks: {
                show: true
            },
            axisBorder: {
                show: true,
                color: "#FEB019"
            },
            labels: {
                style: {
                    colors: ["#FEB019"]
                }
            },
            title: {
                text: "Humid",
                style: {
                    color: "#FEB019"
                }
            },
            tooltip: {
                enabled: true
            }
        },
    ],
    legend: {
        show: false
    },
};
var chart = new ApexCharts(document.querySelector("#chart"), options);
chart.render();

Event.on("humidAndTime",function(event){
    pushData( JSON.parse( event.data ) );
});


const pushData = function(_data){
    tempData.push( parseFloat(_data.temperature).toFixed(2) );
    humidityData.push( parseFloat(_data.humidity).toFixed(2) );
    timeData.push(_data.timeStamp*1000);
    chart.updateOptions( {
        xaxis: {
            categories: timeData
        }
    });
    chart.updateSeries([{ data: tempData }, { data: humidityData }]);
}


const downloadExistingData = async function(){
    const resp = await fetch('/db/download?path=/data.json');
    if( resp.ok ){
        const textData = await resp.text();
        const splittedData = textData.split("\n");
        console.log("splittedData: ", splittedData);
        for (const splitted of splittedData) {
            if(!splitted){ continue; }
            pushData( JSON.parse( splitted ) );
        }
    }
}
downloadExistingData();