// JSON-RPC over Websocket implementation
var JSONRPC_TIMEOUT_MS = 1000;
var ws,pending = {};
var rpcid = 0;
var recv ={
  "dev_num":0,
  "dev_type":1,
  "dev_name":2,
  "output_num":3,
  "output_size":4,
  "switch_state":5,
  "cur_value":6,
  "vol_value":7,
  "pow_value":8,
  "acpow_value":9,
  "factor_value":10,
};


var output ={
  output_num : new Array(42),
  output_size : new Array(42),
  cur_value : new Array(42),
  vol_value : new Array(42),
  pow_value : new Array(42),
  acpow_value : new Array(42),
  factor_value : new Array(42),

};

var jsonrpc = function()
{
  var url = 'ws://localhost:8000/websocket';
  ws = new WebSocket(url);
  if (!ws) return null;
  var type = 0;
  ws.onclose = function(){};
  ws.onmessage = function(evt) {
    // if (frame.id !== undefined) {
    //   if (pending[frame.id] !== undefined) pending[frame.id](frame);  // Resolve
    //   delete (pending[frame.id]);
    // } else {
    // }

    type = JSON.parse(evt.data).result[2];
    if(type  == "output_size")
    {
      output_num = JSON.parse(evt.data).result[3];
    }
    else if(type  == "output_name"){
      var i = JSON.parse(evt.data).result[1];
      output_name[i] = JSON.parse(evt.data).result[3];
      console.log(output_name[i]);
    }
    tick--;
    if(tick == 0)
    {
      push = 1;
    }
  }
  ws.onopen = function(){
    // ws.send("start");
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