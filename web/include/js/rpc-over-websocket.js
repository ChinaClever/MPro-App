// JSON-RPC over Websocket implementation
var JSONRPC_TIMEOUT_MS = 1000;
var ws,pending = {};
var rpcid = 0;
var value_now = 0, val = 0,tick = 0,push = 0;
var user_name='user_name';
var password = 'password';
var identify = '';
var type_info = new Array("Phase","Loop","Output");
var type_name = new Array("","Phs","Loop","","","","TH","sensor","","","Output","Uut","Num","Cfg","User");
var data_type = new Array("","Sw","Vol","Cur","Pow","Enger","Pf","AVpow","React","","","Tmp","Hum","","","","","","","","","Door1","Door2","Water","Smoke");
var data_name = new Array("Size","Val","Rated","Alarm","Max","Min","Vcmin","Vcmax","Enable");
var alarm_name = new Array("","State","Mode","Alarm","Delay");
var cfg_name = new Array("Offline","Serial","AuxNum","ModbusAddr","Version","Buz","Freq","BoardNum");
var uut_name = new Array("IdcName","RoomName","ModuleName","CabinetName","LoopName","DevName");
var user_info = new Array("UserName","Password","Identify");
var jsonrpc = function()
{
  var url = 'ws://localhost:8000/websocket';
  ws = new WebSocket(url);
  if (!ws) return null;
  var type = 0,topic = 0,subtopic = 0,addr = 0,num = 0;
  ws.onclose = function(){};
  ws.onmessage = function(evt) {
    addr  = parseInt(JSON.parse(evt.data).result[0]);
    type = parseInt(JSON.parse(evt.data).result[1]);
    topic = parseInt(JSON.parse(evt.data).result[2]);
    subtopic = parseInt(JSON.parse(evt.data).result[3]);
    num = parseInt(JSON.parse(evt.data).result[4]);
    switch(type)
    {
      case 0:
      break;
      case 1:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 2:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 3:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 4:

      break;
      case 5:

      break;
      case 6:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 7:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5]));
      break;
      case 10:
        sessionStorage.setItem("output_name"+ addr ,JSON.parse(evt.data).result[5]); 
      break;
      case 11:
        sessionStorage.setItem(uut_name[num]+ addr ,JSON.parse(evt.data).result[5]); 
      break;
      case 12:
        sessionStorage.setItem(type_info[topic - 1]+ "Num" + addr +'_'+num ,JSON.parse(evt.data).result[5]);
      break;
      case 13:
        sessionStorage.setItem(cfg_name[topic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5]));
      case 14:
        sessionStorage.setItem(user_info[num] , JSON.parse(evt.data).result[5]);
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