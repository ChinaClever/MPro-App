// JSON-RPC over Websocket implementation
var JSONRPC_TIMEOUT_MS = 1000;
let verfity = 0;
let ws,pending = {};
let rpcid = 0;
let value_now = 0, val = 0;
let user_name='user_name';
let password = 'password';
let identify = '';
let type_info = new Array("Phase","Loop","Output");
let type_name = new Array("Total","Phs","Loop","Group","Double","","TH","Sensor","","","Output","Uut","Num","Cfg","User","Modbus","Snmp","Rpc","Push");
let data_type = new Array("","Sw","Vol","Cur","Pow","Enger","Pf","AVpow","React","","","Tmp","Hum","","","","","","","","","Door1","Door2","Water","Smoke");
let data_name = new Array("Size","Val","Rated","Alarm","Max","Min","Vcmin","Vcmax","Enable");
let alarm_name = new Array("","State","Mode","","Seq","Reset","","","Enable");
let cfg_name = new Array("Offline","Serial","DevMode","SlaveNum","ModbusAddr","Buz","Freq","BoardNum","GroupSwEn","LastRunTime","RunTime");
let uut_name = new Array("","IdcName","RoomName","ModuleName","CabinetName","LoopName","DevName");
let user_info = new Array("","UserName","Password","Identify","","","","","","","","Verfity");
let log_info = new Array("","LogNum","LogInfo");
let modbus_info = new Array("","Enable","Addr","Baud","Parity","Data","Stop","","","","","TcpEnable","TcpPort");
let snmp_info = new Array("","Trap1","Trap2","V3Enable","Username","Password","Key");
let rpc_info = new Array("","JsonMode","JsonPort","XmlMode","XmlPort");
let push_info = new Array("","Udp1En","Udp1Addr","Udp1Port","Udp2En","Udp2Addr","Udp2Port","CtrlMode","Ctrlport","Delay","","PushEn","HttpAddr","PushDelay","RecEncrypt","RecvProt");
let ver_name = new Array("CoreVer","CoreCompile","ThreadVer","ThreadCompile","UIVer","UICompile","Borad2Ver","Borad3Ver","Borad4Ver");
let url_1;
var jsonrpc = function()
{
  var pro = 0;
  var pro_  = window.location.protocol;
  var url_ = window.location.host;
  url_1= window.location.origin;
  if(pro_ == "http:"){
    pro = "ws://";
  }else if(pro_ == "https:"){
    pro = "wss://";
  }url = pro + url_ +'/websocket';
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
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 1:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 2:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 3:
        if(topic == 1){
          sessionStorage.setItem(type_name[type]+ data_type[topic] + alarm_name[subtopic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
        }else{
          sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
        }
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 4:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 5:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 6:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 7:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr +'_'+num, parseInt(JSON.parse(evt.data).result[5]));
      break;
      case 10:
        sessionStorage.setItem("Output_name"+ addr +'_'+num,JSON.parse(evt.data).result[5]); 
      break;
      case 11:
        sessionStorage.setItem(uut_name[topic]+ addr ,JSON.parse(evt.data).result[5]); 
      break;
      case 12:
        sessionStorage.setItem(type_info[topic - 1]+ "Num" + addr ,JSON.parse(evt.data).result[5]);
      break;
      case 13:
        sessionStorage.setItem(cfg_name[topic] + addr, parseInt(JSON.parse(evt.data).result[5]));
      break;
      case 14:
        if(topic  == 11)
        {
          verfity = 1;
        }
        sessionStorage.setItem(user_info[topic] , JSON.parse(evt.data).result[5]);
      break;
      case 15:
        sessionStorage.setItem(type_name[type]+ modbus_info[topic], JSON.parse(evt.data).result[5]);
      break;
      case 16:
        sessionStorage.setItem(type_name[type]+ snmp_info[topic], JSON.parse(evt.data).result[5]);
      break;
      case 17:
        sessionStorage.setItem(type_name[type]+ rpc_info[topic], JSON.parse(evt.data).result[5]);
      break;
      case 18:
        if(topic == 0){
          sessionStorage.setItem(type_name[type]+ push_info[topic], JSON.parse(evt.data).result[5]);
        }else{
          sessionStorage.setItem(type_name[type]+ push_info[topic], JSON.parse(evt.data).result[5]);
        }
      break;
      case 30:
        sessionStorage.setItem(ver_name[topic] + addr, parseInt(JSON.parse(evt.data).result[5]));
      break;
      case 51:
      break;
      case 81:
        sessionStorage.setItem(log_info[subtopic] , JSON.parse(evt.data).result[5]);
      break;
      default:
        break;
    }
  }
  ws.onopen = function(){
      read_user_info();
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
}


var rpc = jsonrpc();
var start  = 0;
var hum_num = 2,num_num = 3,cfg_num = 11,uut_num = 6, sub_num = 8;
var total = 0, phase  = 1,loop = 2,output = 3,envir = 6,sensor = 7,bit = 10,uut = 11,num =12, cfg = 13,user  = 14,modbus = 15,snmp = 16,rpc_cfg = 17,push = 18,ver_ = 30,log = 81;
var switch_ = 1,vol_ = 2,cur_ = 3,pow_ = 4,energe_ = 5,pf_ = 6,AVpow_ = 7,reactpow_ = 8,tmp_ = 11, hum_ = 12, door1_ = 21,door2_ = 22,water_ = 23,smoke_ =24;
var idc_ = 1,room_ = 2;module_ = 3,cabnite_ = 4, loop_ = 5, dev_ = 6;
window.addr = 0;


function read_user_info(){
  var j = 1;
  var time1 = setInterval(function(){
    if(j >= 4){
      clearInterval(time1);
    }
    if(j < 4 ){
      rpc.call('pduReadParam',[0,user,j,0,0]);
    }
    j++;
  },1);
}
function read_dev_name(){
  var slave_num = parseInt(sessionStorage.getItem("SlaveNum" + 0));
  var j = 0;
  var time1 = setInterval(function(){
    if(j >= parseInt(slave_num)){
      clearInterval(time1);
    }
    if(j < slave_num){
      rpc.call('pduReadParam',[j,uut,dev_,0,0]);
    }
    j++;
  },1);
}
function read_uut_info(addr)
{
  var j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(uut_num + 1)){
      clearInterval(time1);
    }
    if(j <= uut_num){
      rpc.call('pduReadParam',[addr,uut,j,0,0]);
    }
    j++;
  },1);
}
function read_total_data(addr){
  var j = 1;
  var time1 = setInterval(function(){
    if(j >= 9){
      clearInterval(time1);
    }
    if(j < 9){
      rpc.call('pduReadData',[addr,total,j,1,1]);
    }
    j++;
  },1);
}
function read_phase_data(addr)
{
  var phase_num = parseInt(sessionStorage.getItem('PhaseNum' + addr));
  var j = 1;var i = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(sub_num + 1)){
      clearInterval(time1);
    }
    if(i <= phase_num && j <= sub_num){
      rpc.call('pduReadData',[addr,phase,vol_,j,i]);
      rpc.call('pduReadData',[addr,phase,cur_,j,i]);
      rpc.call('pduReadData',[addr,phase,pow_,j,i]);
      rpc.call('pduReadData',[addr,phase,pf_,j,i]);
    }
    i++;
    if(i >= (phase_num + 1)){
      i = 1;
      j++;
    }
  },1);
}
function read_loop_data(addr)
{
  var loop_num = parseInt(sessionStorage.getItem('LoopNum' + addr));
  var j = 1;var i = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(sub_num + 1)){
      clearInterval(time1);
    }
    if(i <= loop_num && j <= sub_num){
      rpc.call('pduReadData',[addr,loop,switch_,j,i]);
      rpc.call('pduReadData',[addr,loop,vol_,j,i]);
      rpc.call('pduReadData',[addr,loop,cur_,j,i]);
      rpc.call('pduReadData',[addr,loop,pow_,j,i]);
      rpc.call('pduReadData',[addr,loop,energe_,j,i]);
      rpc.call('pduReadData',[addr,loop,AVpow_,j,i]);
    }
    i++;
    if(i >= (loop_num + 1)){
      i = 1;
      j++;
    }
  },1);
}
function read_output_data(addr)
{
  var output_num = parseInt(sessionStorage.getItem('OutputNum' + addr));
  var j = 1;var i = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(sub_num + 1)){
      clearInterval(time1);
    }
    if(i <= output_num && j <= sub_num){
      if(j == 1 && i <= output_num){
        rpc.call('pduReadData',[addr,output,energe_,j,i]);
        rpc.call('pduReadData',[addr,output,pf_,j,i]);
        rpc.call('pduReadData',[addr,output,AVpow_,j,i]);
        rpc.call('pduReadData',[addr,output,reactpow_,j,i]);
      }
      if((j == 1 || j == 2 || j == 4 || j == 5 || j == 8) && i <= output_num){
        rpc.call('pduReadData',[addr,output,switch_,j,i]);
      }
      rpc.call('pduReadData',[addr,output,cur_,j,i]);
      rpc.call('pduReadData',[addr,output,pow_,j,i]);
    }
      i++;
      if(i >= parseInt(output_num + 1)){
        i = 1;
        j++;
      }
  },1);
}
function read_sensor_data(addr)
{
  var j = 1;var i = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(sub_num + 1)){
      clearInterval(time1);
    }
    if(i <= hum_num && j <= sub_num){
      if((j == 1) && i == 1){
        rpc.call('pduReadData',[addr,sensor,door1_,j,i]);
        rpc.call('pduReadData',[addr,sensor,door2_,j,i]);
        rpc.call('pduReadData',[addr,sensor,water_,j,i]);
        rpc.call('pduReadData',[addr,sensor,smoke_,j,i]);
      }
      rpc.call('pduReadData',[addr,envir,tmp_,j,i]);
      rpc.call('pduReadData',[addr,envir,hum_,j,i]);
      i++;
      if(i >= parseInt(hum_num + 1)){
        i = 1;
        j++;
      }
    }
  },1);
}
function read_num_info(addr){
  var j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(num_num + 1)){
      clearInterval(time1);
    }
    if(j <= num_num){
      rpc.call('pduReadParam',[addr,num,j,0,0]);
    }
    j++;
  },1);
}
function read_cfg_info(addr){
  var j = 0;
  var time1 = setInterval(function(){
    if(j >= parseInt(cfg_num)){
      clearInterval(time1);
    }
    if(j < cfg_num){
      rpc.call('pduReadParam',[addr,cfg,j,0,0]);
    }
    j++;
  },1);
}
function read_output_name(addr){
  var output_num = parseInt(sessionStorage.getItem('OutputNum' + addr));
  var j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(output_num + 1)){
      clearInterval(time1);
    }
    if(j <= output_num){
      rpc.call('pduReadParam',[addr,bit,0,0,j]);
    }
    j++;
  },1);
}
function read_modbus_data(){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(13)){
      clearInterval(time1);
    }
    if(j <= 12 && (j < 7 ||(j>10))){
      rpc.call('pduReadParam',[addr,modbus,j,0,0]);
    }
    j++;
  },1);
}
function read_rpc_data(){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(6)){
      clearInterval(time1);
    }
    if(j <= 5 ){
      if(j != 3){
       rpc.call('pduReadParam',[addr,rpc_cfg,j,0,0]);
      }
    }
    j++;
  },1);
}

function read_snmp_data(){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(7)){
      clearInterval(time1);
    }
    if(j <= 6 ){
      rpc.call('pduReadParam',[addr,snmp,j,0,0]);
    }
    j++;
  },1);
}
function read_log_data(type,name,start,num){
  rpc.call('pduLogFun',[start,log,type,name,num]);
}
function read_push_data(){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(10)){
      clearInterval(time1);
    }
    if(j <= 9 ){
      rpc.call('pduReadParam',[addr,push,j,0,0]);
    }
    j++;
  },1);


}
function read_http_data(){
  let j = 11;
  var time1 = setInterval(function(){
    if(j >= parseInt(16)){
      clearInterval(time1);
    }
    if(j <= 15){
      rpc.call('pduReadParam',[addr,push,j,0,0]);
    }
    j++;
  },1);
}
function read_ver_info(){
  let j = 0;
  var time1 = setInterval(function(){
    if(j >= parseInt(11)){
      clearInterval(time1);
    }
    if(j <= 10){
      rpc.call('pduReadParam',[addr,ver_,j,0,0]);
    }
    j++;
  },1);
}