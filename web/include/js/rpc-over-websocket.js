// JSON-RPC over Websocket implementation
var JSONRPC_TIMEOUT_MS = 1000;
var ws,pending = {};
var rpcid = 0;
var recv ={
  "dev_num":0,
  "phase":1,
  "loop":2,
  "output":3,
  "recv1":4,
  "recv2":5,
  "cur_value":6,
  "vol_value":7,
  "pow_value":8,
  "acpow_value":9,
  "factor_value":10,
};
var value_now = 0, val = 0,tick = 0,push = 0;
var output_name = new Array();
var dev_num = 0,output_num = 0;
var cur_val = new Array();
var cur_rated = new Array();
var cur_max = new Array();
var cur_min = new Array();
var vcrmax = new Array();
var vcrmin = new Array();
var cur_alarm = new Array();
var cur_enable = new Array();
var dev_type = new Array();
var output_size = new Array();
var swtich_state = new Array();

var jsonrpc = function()
{
  var url = 'ws://localhost:8000/websocket';
  ws = new WebSocket(url);
  if (!ws) return null;
  var type = 0,topic = 0,subtopic = 0;
  ws.onclose = function(){};
  ws.onmessage = function(evt) {
    type = parseInt(JSON.parse(evt.data).result[1]);
    topic = parseInt(JSON.parse(evt.data).result[2]);
    subtopic = parseInt(JSON.parse(evt.data).result[3]);
    switch(type)
    {
      case 0:
      break;
      case 1:

      break;
      case 2:

      break;
      case 3:
        if(topic == 0)
        {
          output_num = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 1)
        {
          swtich_state[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 2)
        {
          vol_val[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 3)
        {
          if(subtopic == 1){
            cur_val[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
          }
          else if(subtopic == 2){
            cur_rated[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
          }
          else if(subtopic == 3){
            cur_alarm[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
          }
          else if(subtopic == 4){
            cur_max[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
          }
          else if(subtopic == 5){
            cur_min[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
          }
          else if(subtopic == 6){
            vcrmin[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
          }
          else if(subtopic == 7){
            vcrmax[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
          }
          else if(subtopic == 8){
            cur_enable[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
          }
        }
        else if(topic == 4)
        {
          pow_val[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 5)
        {
          pf_val[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 6)
        {
          energe_val[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 11)
        {
          tem_val[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 12)
        {
          hum_val[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 21)
        {
          door1[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 22)
        {
          door2[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 23)
        {
          water[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 24)
        {
          smoke[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
      break;
      case 4:

      break;
      case 5:

      break;
      case 6:
        
      break;
      case 7:
      
      break;
      case 10:
        output_name[parseInt(JSON.parse(evt.data).result[4])] = JSON.parse(evt.data).result[5];
      break;
      default:
        break;
    }
    tick--;
    if(tick == 0)
    {
      push = 1;
    }
  }
  ws.onopen = function(){
  };
  return {
    close:() => ws.close(),
    call:function(method,params){
      const id = rpcid++,request = {id, method, params};
      ws.send(JSON.stringify(request));
      return new Promise(function(resolve, reject) {
        setTimeout(JSONRPC_TIMEOUT_MS, function() {
          if (pending[id] === undefined) return;
          delete (pending[id]);
          reject();
        });
        pending[id] = x => resolve(x);
      });
    },
  };
};