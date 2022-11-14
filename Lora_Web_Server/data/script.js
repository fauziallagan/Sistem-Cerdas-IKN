//Get current sensor readings when the page loads  
window.addEventListener('load', getReadings);
let widthGauge = 170;
let heightGauge = 170;
let majorTickVarNatrium = ["0",
  "25",
  "50",
  "75",
  "100",
  "125",
  "150",
  "175",
  "200",
  "225",
  "255"
]
let majorTickPotasium = [
  "0",
  "25",
  "50",
  "75",
  "100",
  "125",
  "150",
  "175",
  "200",
  "225",
  "255"
]
let majorTickKalium = ["0",
  "25",
  "50",
  "75",
  "100",
  "125",
  "150",
  "175",
  "200",
  "225",
  "255"
]
let majorTickpH = ["0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "10",
  "11",
  "12",
  "13",
  "14"
]
let majorTickSoil = ["0",
  "10",
  "20",
  "30",
  "40",
  "50",
  "60",
  "70",
  "80",
  "90",
  "100"
]
let heightNatrium = [{
    "from": 0,
    "to": 45,
    "color": "#D4DBA1"
  },
  {
    "from": 45,
    "to": 90,
    "color": "#CFDE64"
  }, {
    "from": 90,
    "to": 140,
    "color": "#D0E43B"
  }, {
    "from": 140,
    "to": 255,
    "color": "#E0FF00"
  }
]
let heightPotasium = [{
    "from": 150,
    "to": 255,
    "color": "#f20202"
  },
  {
    "from": 0,
    "to": 45,
    "color": "#dbd7cc"
  }, {
    "from": 45,
    "to": 90,
    "color": "#f2ee02"
  }, {
    "from": 90,
    "to": 150,
    "color": "#02f262"
  }
]
let heightKalium = [{
    "from": 150,
    "to": 255,
    "color": "#f20202"
  },
  {
    "from": 0,
    "to": 45,
    "color": "#dbd7cc"
  }, {
    "from": 45,
    "to": 90,
    "color": "#f2ee02"
  }, {
    "from": 90,
    "to": 150,
    "color": "#02f262"
  }
]
let heightpH = [{
    "from": 0,
    "to": 2,
    "color": "#E62300"
  },
  {
    "from": 2,
    "to": 4,
    "color": "#DE4B31"
  }, {
    "from": 4,
    "to": 6,
    "color": "#DB6B57"
  },
  {
    "from": 6,
    "to": 8,
    "color": "#A5EA0F"
  },
  {
    "from": 8,
    "to": 10,
    "color": "#9AD321"
  },
  {
    "from": 10,
    "to": 14,
    "color": "#3585DE"
  }
]
let heightSoil = [{
    "from": 0,
    "to": 20,
    "color": "#C2D2EE"
  },
  {
    "from": 20,
    "to": 60,
    "color": "#8EB2EC"
  }, {
    "from": 60,
    "to": 80,
    "color": "#5B92ED"
  }, {
    "from": 80,
    "to": 100,
    "color": "#4082EE"
  }
]
let colorPlatevar = "#fff";
let needleTypeVar = 'arrow';
let minorTicksVar = 10;

var natriumLahan1Gauge = new RadialGauge({
  renderTo: 'natriumLahan1',
  width: widthGauge,
  height: heightGauge,
  units: "ppm",
  minValue: 0,
  maxValue: 255,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueInt: 0,
  majorTicks: majorTickVarNatrium,
  minorTicks: minorTicksVar, // edited 4
  strokeTicks: true,
  highlights: heightNatrium,
  colorPlate: colorPlatevar,
  borderShadowWidth: 1, // edited 0
  borders: true,
  colorNeedle: "#007F80",
  colorNeedleEnd: "#007F80",
  needleWidth: 2,
  needleCircleSize: 3,
  colorNeedleCircleOuter: "#007F80",
  needleCircleOuter: true,
  needleCircleInner: false,
  needleType: needleTypeVar,
  animationDuration: 500, // edited 1500
  animationRule: "bounce"
}).draw();

var potasiumLahan1Gauge = new RadialGauge({
  renderTo: 'potasiumLahan1',
  width: widthGauge,
  height: heightGauge,
  units: "ppm",
  minValue: 0,
  maxValue: 255,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueInt: 1,
  majorTicks: majorTickPotasium,
  minorTicks: minorTicksVar, // edited 4
  strokeTicks: true,
  highlights: heightPotasium,
  colorPlate: colorPlatevar,
  borderShadowWidth: 1, // edited 0
  borders: true,
  colorNeedle: "#007F80",
  colorNeedleEnd: "#007F80",
  needleWidth: 2,
  needleCircleSize: 3,
  colorNeedleCircleOuter: "#007F80",
  needleCircleOuter: true,
  needleCircleInner: false,
  needleType: needleTypeVar,
  animationDuration: 500, // edited 1500
  animationRule: "bounce"
}).draw();

var kaliumLahan1Gauge = new RadialGauge({
  renderTo: 'kaliumLahan1',
  width: widthGauge,
  height: heightGauge,
  units: "ppm ",
  minValue: 0,
  maxValue: 255,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueInt: 1,
  majorTicks: majorTickKalium,
  minorTicks: minorTicksVar, // edited 4
  strokeTicks: true,
  highlights: heightKalium,
  colorPlate: colorPlatevar,
  borderShadowWidth: 1, // edited 0
  borders: true,
  colorNeedle: "#007F80",
  colorNeedleEnd: "#007F80",
  needleWidth: 2,
  needleCircleSize: 3,
  colorNeedleCircleOuter: "#007F80",
  needleCircleOuter: true,
  needleCircleInner: false,
  needleType: needleTypeVar,
  animationDuration: 500, // edited 1500
  animationRule: "bounce"
}).draw();

var KelembabanLahan1Gauge = new RadialGauge({
  renderTo: 'KelembabanLahan1',
  width: widthGauge,
  height: heightGauge,
  units: "Humidity (%)",
  minValue: 0,
  maxValue: 100,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueInt: 1,
  majorTicks: majorTickSoil,
  minorTicks: minorTicksVar, // edited 4
  strokeTicks: true,
  highlights: heightSoil,
  colorPlate: colorPlatevar,
  borderShadowWidth: 1, // edited 0
  borders: true,
  colorNeedle: "#007F80",
  colorNeedleEnd: "#007F80",
  needleWidth: 2,
  needleCircleSize: 3,
  colorNeedleCircleOuter: "#007F80",
  needleCircleOuter: true,
  needleCircleInner: false,
  needleType: needleTypeVar,
  animationDuration: 500, // edited 1500
  animationRule: "bounce"
}).draw();

var phLahan1Gauge = new RadialGauge({
  renderTo: 'phLahan1',
  width: widthGauge,
  height: heightGauge,
  units: "",
  minValue: 0,
  maxValue: 14,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueInt: 1,
  majorTicks: majorTickpH,
  minorTicks: minorTicksVar, // edited 4
  strokeTicks: true,
  highlights: heightpH,
  colorPlate: colorPlatevar,
  borderShadowWidth: 1, // edited 0
  borders: true,
  colorNeedle: "#007F80",
  colorNeedleEnd: "#007F80",
  needleWidth: 2,
  needleCircleSize: 3,
  colorNeedleCircleOuter: "#007F80",
  needleCircleOuter: true,
  needleCircleInner: false,
  needleType: needleTypeVar,
  animationDuration: 500, // edited 1500
  animationRule: "bounce"
}).draw();


// Function to get current readings on the webpage when it loads for the first time
function getReadings() {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);

      let natriumLahan1Var = myObj.natriumLahan1FromArduino;
      let kelembabanLahan1Var = myObj.kelembabanLahan1FromArduino;
      let potasiumLahan1Var = myObj.potasiumLahan1FromArduino;
      let kaliumLahan1Var = myObj.kaliumLahan1FromArduino;
      let phLahan1Var = myObj.phLahan1FromArduino;

      KelembabanLahan1Gauge.value = kelembabanLahan1Var;
      natriumLahan1Gauge.value = natriumLahan1Var;
      potasiumLahan1Gauge.value = potasiumLahan1Var;
      kaliumLahan1Gauge.value = kaliumLahan1Var;
      phLahan1Gauge.value = phLahan1Var;



      // console.log(kelembabanLahan1Var);
      // console.log(natriumLahan1Var);
      // console.log(potasiumLahan1Var);
      // console.log(kaliumLahan1Var);
      // console.log(phLahan1Var);

    }
  };

  xhr.open("GET", "/readings", true);
  xhr.send();
}

if (!!window.EventSource) {
  var source = new EventSource('/events');

  source.addEventListener('open', function (e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function (e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");

    }
  }, false);

  source.addEventListener('message', function (e) {
    console.log("message", e.data);
  }, false);

  source.addEventListener('new_readings', function (e) {
    console.log("new_readings", e.data);
    var myObj = JSON.parse(e.data);
    // console.log(myObj);

    KelembabanLahan1Gauge.value = myObj.kelembabanLahan1FromArduino;
    kaliumLahan1Gauge.value = myObj.kaliumLahan1FromArduino;
    potasiumLahan1Gauge.value = myObj.potasiumLahan1FromArduino;
    natriumLahan1Gauge.value = myObj.natriumLahan1FromArduino;
    phLahan1Gauge.value = myObj.phLahan1FromArduino

    let sensorKel = KelembabanLahan1Gauge.value;
    console.log(sensorKel);

    if (sensorKel >= 41 && sensorKel <= 100) {
      document.getElementById("kelembaban").value = "Status : Basah";
      console.log("basah");
    } else {
      document.getElementById("kelembaban").value = "Status : Kering";
      console.log("Kering");
    }
    let sensorpH = phLahan1Gauge.value;
    console.log(sensorpH);
    if (sensorpH <= 6) {
      document.getElementById("pH").value = "Status : Asam";
      console.log("ASAM");
    } else if (sensorpH >= 7 && sensorpH <= 9) {
      document.getElementById("pH").value = "Status : Netral";
      console.log("Netral");
    } else if (sensorpH >= 10 && sensorpH <= 14) {
      document.getElementById("pH").value = "Status : Basa";
      console.log("Basa");
    }


    // gaugeSoil_D.value = myObj.moisture;
  }, false);
}