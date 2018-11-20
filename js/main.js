function leftPad(number, targetLength) {
    var output = number + '';
    while (output.length < targetLength) {
        output = '0' + output;
    }
    return output;
}


function addrow(index, row) {

    var tr = $("#mtable_file tbody tr:last").clone();
    tr.attr("id", "tr_" + index);

    //tr.css("opacity", "0.3");
    //tr.css("filter", "alpha(Opacity=30)");


    $('#mtable_file tbody').append(tr);
    //$('#mtable_file tbody').append($("#mtable_file tbody tr:last").clone());

    //$('#mtable_file tbody tr:last :checkbox').attr('checked',false);
    $('#mtable_file tbody tr:last').show();

    var d = new Date();
    //var date_text = new Date(parseInt(row['time_stamp'])).toISOString();

    var date_text = leftPad(d.getHours(), 2) + ":" + leftPad(d.getMinutes(), 2) + ":" + leftPad(d.getSeconds(), 2) + " " + leftPad(d.getDate(), 2) + "-" + leftPad((d.getMonth() + 1), 2) + "-" + leftPad(d.getFullYear(), 4);



    $('#mtable_file tbody tr:last td:nth-child(1)').html(index);
    $('#mtable_file tbody tr:last td:nth-child(2)').html(row["id"]);
    $('#mtable_file tbody tr:last td:nth-child(3)').html(row["dx"]);
    $('#mtable_file tbody tr:last td:nth-child(4)').html(row["dy"]);
    $('#mtable_file tbody tr:last td:nth-child(5)').html(date_text);

}
;

function updaterow(row) {
    var d = new Date();
    var date_text = +leftPad(d.getHours(), 2) + ":" + leftPad(d.getMinutes(), 2) + ":" + leftPad(d.getSeconds(), 2) + " " + leftPad(d.getDate(), 2) + "-" + leftPad((d.getMonth() + 1), 2) + "-" + leftPad(d.getFullYear(), 4);

    $("#tr_" + row["id"] +" td:nth-child(2)").html(row["id"]);
    $("#tr_" + row["id"] +" td:nth-child(3)").html(row["dx"]);
    $("#tr_" + row["id"] +" td:nth-child(4)").html(row["dy"]);
    $("#tr_" + row["id"] +" td:nth-child(5)").html(date_text);
}

function del_tr(index) {

    $('#tr_' + index).fadeOut(800, function () {
        $(this).remove();
    });

    //$('#tr_'+index).slideUp("slow", function() { $(this).remove();});
    //$('#tr_'+index).remove();


}
;

_sensors = JSON.parse('[{"id":1, "dx": 1, "dy": 1}, {"id": 2,"dx": 2, "dy": 2}]');
_sensors1 = JSON.parse('[{"id":1, "dx": 11, "dy": 11}, {"id": 2,"dx": 22, "dy": 22}]');

function UpdateSensor(Sensors) {

    for (var key in Sensors) {
        if ($("#tr_" + Sensors[key]["id"]).length) {
            updaterow(Sensors[key]);
        } else {
            addrow(Sensors[key]["id"], Sensors[key])
        }
    }

}
;

function UpdateCalibration(Sensors) {

    //$("#val_calibration").val( JSON.stringify(Sensors));
    
    $("#c1_alpha1").html( JSON.stringify(Sensors["c1_alpha1"]));
    $("#c1_beta1").html( JSON.stringify(Sensors["c1_beta1"]));
    $("#c1_alpha2").html( JSON.stringify(Sensors["c1_alpha2"]));
    $("#c1_beta2").html( JSON.stringify(Sensors["c1_beta2"]));
    $("#c1_alpha3").html( JSON.stringify(Sensors["c1_alpha3"]));
    $("#c1_beta3").html( JSON.stringify(Sensors["c1_beta3"]));
    
    $("#c2_alpha1").html( JSON.stringify(Sensors["c2_alpha1"]));
    $("#c2_beta1").html( JSON.stringify(Sensors["c2_beta1"]));
    $("#c2_alpha2").html( JSON.stringify(Sensors["c2_alpha2"]));
    $("#c2_beta2").html( JSON.stringify(Sensors["c2_beta2"]));
    $("#c2_alpha3").html( JSON.stringify(Sensors["c2_alpha3"]));
    $("#c2_beta3").html( JSON.stringify(Sensors["c2_beta3"]));
    

}
;


////////////////////////////////////////////////////////////////////////////////

var ip = location.host;
var WsUri = "ws://" + ip + "/ws"; //"ws://" + ip + "/ws";var output;
var cam_socket;
var img;
var canvas;
var ctx;

function init()
{
    output = document.getElementById("output");
    img = document.createElement("img");
    //img.height = 640;
    //img.height = windows.innerHeight;
    img.src = "";
    img.id = "ws-image";
    output.appendChild(img);
    InitWebSocket();
    //window.setTimeout(sendCommandVector, 1000);
    //canvas = img;
    //ctx = canvas.getContext("2d");
    //ctx.font = "30px Arial";
    //ctx.fillStyle = "red";
}

function InitWebSocket()
{
    console.log("WsUri: " + WsUri);
    cam_socket = new WebSocket(WsUri);
    cam_socket.onopen = function (evt) {
        onOpen(evt)
    };
    cam_socket.onclose = function (evt) {
        onClose(evt)
    };
    cam_socket.onmessage = function (evt) {
        onMessage(evt)
    };
    cam_socket.onerror = function (evt) {
        onError(evt)
    };

}

function onOpen(evt)
{
    writeToScreen("CAM CONNECTED");
}
function onClose(evt)
{
    writeToScreen("CAM DISCONNECTED");
}
function onMessage(evt)
{
    console.log(evt.data);
    _sensors = JSON.parse(evt.data);
    UpdateSensor(_sensors["coordinates"]);
    UpdateCalibration(_sensors["calibration"]);
    
}
function onError(evt)
{
    writeToScreen('<span style="color: red;">ERROR:</span> ' + evt.data);
}

function writeToScreen(message)
{
    
    console.log(message);
    //var pre = document.createElement("p");
    //pre.style.wordWrap = "break-word";
    //pre.innerHTML = message;
    //output.appendChild(pre);

    //ctx.fillText(message,10,50);
}



InitWebSocket();