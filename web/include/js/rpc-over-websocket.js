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
          swtich_state[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 3)
        {
          cur_size[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 4)
        {
          swtich_state[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 5)
        {
          swtich_state[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 6)
        {
          swtich_state[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 7)
        {
          swtich_state[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 8)
        {
          swtich_state[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 9)
        {
          swtich_state[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 10)
        {
          swtich_state[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
        }
        else if(topic == 11)
        {
          swtich_state[parseInt(JSON.parse(evt.data).result[4])] = parseInt(JSON.parse(evt.data).result[5]);
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
    }
    // if(type  == "output_size")
    // {
    //   output_num = JSON.parse(evt.data).result[3];
    // }
    // else if(type  == "output_name"){
    //   var i = JSON.parse(evt.data).result[1];
    //   output_name[i] = JSON.parse(evt.data).result[3];
    //   console.log(output_name[i]);
    // }
    // else if(type  == "switch_state"){
    //   var i = JSON.parse(evt.data).result[1];
    //   swtich_state[i] = JSON.parse(evt.data).result[3];
    // }
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