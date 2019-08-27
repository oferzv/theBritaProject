const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body >

<div id="wrap" style="display: inline-block; text-align:left;">

  <h1>The Brita IoT project</h1>
  
  <div style="float:left;">
    
    SYSTEM TIME:  <span id="SysTime">NA</span>
    <br>
    SCALE VALUE : <span id="ADCValue">0</span>
    <br>
    SYSTEM STATE  : <span id="SYSState">NA</span>
    <br>
    LED State is : <span id="LEDState">NA</span>
    <br><br>

    <button type="button" onclick="sendData(1)">CLOSE LID</button>
    <button type="button" onclick="sendData(0)">OPEN LID</button><BR> 
  </div>
  <div style="float:left; padding-left: 15px;">
    <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/853293/widgets/97382"></iframe>
    <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/853293/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Water+Refill&type=line&yaxis=Water+in+ML"></iframe>

  </div>
  
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
      document.getElementById("SysTime").innerHTML =resArray[2];
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
