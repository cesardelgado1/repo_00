//left shoulder x & y
var ls_x = msg.payload[0].ls_x;
var ls_y = msg.payload[0].ls_y;
//left wrist x & y
var lw_x = msg.payload[0].lw_x;
var lw_y = msg.payload[0].lw_y;
//right shoulder x & y
var rs_x = msg.payload[0].rs_x;
var rs_y = msg.payload[0].rs_y;
//right wrist x & y
var rw_x = msg.payload[0].rw_x;
var rw_y = msg.payload[0].rw_y;
//right hip x & y
var rh_x = msg.payload[0].rh_x;
var rh_y = msg.payload[0].rh_y;
//left hip x & y
var lh_x = msg.payload[0].lh_x;
var lh_y = msg.payload[0].lh_y;
var pointing;
//Prototype code 4
/////////////////////////////////////////////////////////////////
//distance from left sholder to wrist
var distance3 = Math.sqrt(Math.pow(ls_x - lw_x, 2) + Math.pow(ls_y - lw_y, 2));
//distance from right shoulder to wrist
var distance4 = Math.sqrt(Math.pow(rs_x - rw_x, 2) + Math.pow(rs_y - rw_y, 2));
//distance from right wrist to write hip
var distance5 = Math.sqrt(Math.pow(rw_x - rh_x, 2) + Math.pow(rw_y - rh_y, 2));
//distance from left wrist to write hip
var distance6 = Math.sqrt(Math.pow(lw_x - lh_x, 2) + Math.pow(lw_y - lh_y, 2));
/////////////////////////////////////////////////////////////////
if (current_cam == "mycam") {
    if (distance3 > 120 && distance5 < 70 && distance6 > 70) {
        pointing = "left";
    }
    else if (distance4 > 120 && distance6 < 70 && distance5 > 70) {
        pointing = "right";
    }
    else {
        pointing = "nopose"
    }
}

msg.payload = pointing;
return msg;