const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>

<div id="demo">
<h1>The ESP8266 NodeMCU Brita project</h1>
 <button type="button" onclick="sendData(1)">CLOSE LID</button>
  <button type="button" onclick="sendData(0)">OPEN LID</button><BR> 
</div>

<div>
  SCALE VALUE : <span id="ADCValue">0</span><br>
  SYSTEM STATE  : <span id="SYSState">NA</span><br>
  LED State is : <span id="LEDState">NA</span><br>
</div>
<script>
function sendData(led) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("LEDState").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "setLED?LEDstate="+led, true);
  xhttp.send();
}

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 500); //2000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      resArray = this.responseText.split(",");
      document.getElementById("ADCValue").innerHTML =resArray[1];
      document.getElementById("SYSState").innerHTML =resArray[0];
      
    }
  };
  xhttp.open("GET", "readData", true);
  xhttp.send();
}

</script>



</body>
</html>
)=====";
