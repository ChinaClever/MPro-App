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
var user_name='user_name';
var password = 'password';
var identify = '';
var output_num1 = 0;

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
        if(topic == 2)
        {

        }
        else if(topic == 3)
        {
          if(subtopic == 1){
            sessionStorage.setItem("cur_val"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 2){
            sessionStorage.setItem("cur_rated"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 3){
            sessionStorage.setItem("cur_alarm"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 4){
            sessionStorage.setItem("cur_max"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 5){
            sessionStorage.setItem("cur_min"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 6){
            sessionStorage.setItem("vcrmin"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 7){
            sessionStorage.setItem("vcrmax"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 8){
            sessionStorage.setItem("cur_enable"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }
        }
      break;
      case 3:
        if(topic == 0)
        {
          output_num1 = parseInt(JSON.parse(evt.data).result[5]);
          sessionStorage.setItem("output_num",parseInt(JSON.parse(evt.data).result[5])); 
        }
        else if(topic == 1)
        {
          if(subtopic == 1){
            sessionStorage.setItem("swtich_state"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          } else if(subtopic == 2){
            sessionStorage.setItem("switch_mode"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 4){
            sessionStorage.setItem("seq_delay"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 8){
            sessionStorage.setItem("switch_enable"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }
        }
        else if(topic == 2)
        {
          sessionStorage.setItem("vol_val"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
        }
        else if(topic == 3)
        {
          if(subtopic == 1){
            sessionStorage.setItem("cur_val"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 2){
            sessionStorage.setItem("cur_rated"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 3){
            sessionStorage.setItem("cur_alarm"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 4){
            sessionStorage.setItem("cur_max"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 5){
            sessionStorage.setItem("cur_min"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 6){
            sessionStorage.setItem("vcrmin"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 7){
            sessionStorage.setItem("vcrmax"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 8){
            sessionStorage.setItem("cur_enable"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }
        }
        else if(topic == 4)
        {
          if(subtopic == 1){
            sessionStorage.setItem("pow_val"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 2){
            sessionStorage.setItem("pow_rated"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 3){
            sessionStorage.setItem("pow_alarm"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 4){
            sessionStorage.setItem("pow_max"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 5){
            sessionStorage.setItem("pow_min"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 6){
            sessionStorage.setItem("vcpowmin"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 7){
            sessionStorage.setItem("vcpowmax"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }else if(subtopic == 8){
            sessionStorage.setItem("pow_enable"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
          }
        }
        else if(topic == 5)
        {
          sessionStorage.setItem("energe_val"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
        }
        else if(topic == 6)
        {
          sessionStorage.setItem("pf_val"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
        }
        else if(subtopic == 7){
          sessionStorage.setItem("acpow_val"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
        }
        else if(subtopic == 8){
          sessionStorage.setItem("reac_pow"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
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
        sessionStorage.setItem("output_name"+ parseInt(JSON.parse(evt.data).result[4]),JSON.parse(evt.data).result[5]); 
      break;
      case 13:
        if(topic == 1){
          sessionStorage.setItem("pow_val"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
        }else if(topic == 2){
          sessionStorage.setItem("slave_num",parseInt(JSON.parse(evt.data).result[5])); 
        }else if(topic == 3){
          sessionStorage.setItem("pow_alarm"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
        }else if(topic == 4){
          sessionStorage.setItem("pow_max"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
        }else if(topic == 5){
          sessionStorage.setItem("pow_min"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
        }else if(topic == 6){
          sessionStorage.setItem("vcpowmin"+ parseInt(sJSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
        }else if(topic == 7){
          sessionStorage.setItem("vcpowmax"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
        }else if(topic == 8){
          sessionStorage.setItem("pow_enable"+ parseInt(JSON.parse(evt.data).result[4]),parseInt(JSON.parse(evt.data).result[5])); 
        }
      case 14:
        if(parseInt(JSON.parse(evt.data).result[4]) == 1){
          user_name = JSON.parse(evt.data).result[5];
        }
        else if(parseInt(JSON.parse(evt.data).result[4]) == 1){
          password = JSON.parse(evt.data).result[5];
        }
        else if(parseInt(JSON.parse(evt.data).result[4]) == 1){
          identify = JSON.parse(evt.data).result[5];
        }
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