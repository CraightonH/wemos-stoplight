const char INDEX_PAGE[] PROGMEM = R"=====(<html>
    <head>
        <title>Stoplight</title>
        <style>
            .stoplight-background {
                background-color: black;
                width: 400px;
                height: 700px;
                border-radius: 40px;
            }
            .stoplight-light {
                width: 200px;
                height:200px;
                border-radius: 50%;
                z-index: 100;
                position: relative;
                align-content: center;
                margin: 20px;
            }
            .stoplight-red {
                background-color: #A00;
            }
            .stoplight-yellow {
                background-color: #AA0;
            }
            .stoplight-green {
                background-color: darkgreen;
            }
            .stoplight-red-on {
                background-color: #F00;
                box-shadow: rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #441313 0 -1px 9px, rgba(255, 0, 0, 0.5) 0 2px 0;
            }
            .stoplight-yellow-on {
                background-color: #FF0;
                box-shadow: rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #808002 0 -1px 9px, #FF0 0 2px 0;
              }
            .stoplight-green-on {
                background-color: #ABFF00;
                box-shadow: rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #304701 0 -1px 9px, #89FF00 0 2px 12px;
              }
            .center {
                margin: auto;
                padding: 10px;
            }
        </style>
        <script type="text/javascript" src="http://code.jquery.com/jquery-1.7.1.min.js"></script>
        <script>
            let checkLight = setInterval(function() { changeLight('status') },1000)
            let APIurl = "/stoplight/"
            let previousLightID = ""
            function setLight(data) {
                divID = data + "Light"
                if (divID != previousLightID) {
                    $("#" + previousLightID).removeClass("stoplight-red-on stoplight-yellow-on stoplight-green-on");
                }
                $("#" + divID).addClass("stoplight-" + data + "-on");
                previousLightID = divID
            }
            function changeLight(endpoint) {
                $.ajax({
                    type: "GET",
                    dataType: "text",
                    url: APIurl + endpoint,
                    success: function(data) {
                        setLight(data)
                    }
                });
            }
        </script>
    </head>
    <body onload="changeLight('status')">
        <div class="stoplight-background center">
            <div id="redLight" class="stoplight-light stoplight-red center" onclick="changeLight('red');"></div>
            <div id="yellowLight" class="stoplight-light stoplight-yellow center" onclick="changeLight('yellow');"></div>
            <div id="greenLight" class="stoplight-light stoplight-green center" onclick="changeLight('green');"></div>
        </div>
        <div class="center">
                <button id="cycle" onclick="changeLight('cycle');">Cycle</button>
                <button id="off" onclick="changeLight('off');">Off</button>
        </div>
    </body>
</html>)=====";
