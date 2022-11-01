// JSON-RPC over Websocket implementation
var JSONRPC_TIMEOUT_MS = 1000;
let verfity = 0;
let ws,pending = {};
let rpcid = 0;
let value_now = 0, val = 0;
let user_name='user_name';
let password = 'password';
let identify = '';
let addr  = 0;
let type_info = new Array("","Phase","Loop","Output","Board","Slave","BoardOutput","","","","","LoopStart","LoopEnd");
let type_name = new Array("Total","Phs","Loop","Output","Group","Dual","TH","Sensor","","","Output","Uut","Num","Cfg","User","Modbus","Snmp","Rpc","Push","Mqtt","","Content","Output","Group","Dual","GroupInfo","GroupSet");
let data_type = new Array("","Sw","Vol","Cur","Pow","Enger","Pf","AVpow","React","","","Tmp","Hum","","","","","","","","","Door1","Door2","Water","Smoke");
let data_name = new Array("Size","Val","Rated","Alarm","Max","Min","Vcmin","Vcmax","Enable");
let alarm_name = new Array("","State","Mode","Alarm","Seq","Reset","Overrun","Timeout","Enable");
let cfg_name = new Array("Offline","Serial","DevState","DevMode","SlaveAddr","RunTime","Freq","Buz","GroupSwEn","EnergeSwEn","PowSwEn","BreakerEn","Direction","Angle");
let uut_name = new Array("","RoomName","AddrInfo","DevName","QRCode","DevSN");
let user_info = new Array("","UserName","Password","Identify","","","","","","","","Verfity");
let log_info = new Array("","LogNum","LogInfo");
let modbus_info = new Array("","Enable","Addr","Baud","Parity","Data","Stop","","","","","TcpEnable","TcpPort");
let snmp_info = new Array("","Trap1","Trap2","V3Enable","Username","Password","Key");
let rpc_info = new Array("","JsonMode","JsonPort","","XmlMode","XmlPort","");
let push_info = new Array("","Udp1En","Udp1Addr","Udp1Port","Udp2En","Udp2Addr","Udp2Port","Delay","CtrlMode","Ctrlport","","PushEn","HttpAddr","PushDelay","RecEncrypt","RecvProt");
let ver_name = new Array("CoreVer","CoreCompile","ThreadVer","ThreadCompile","UIVer","UICompile","Borad2Ver","Borad3Ver","Borad4Ver");
let info_info = new Array("","Name","PowerOn","PowerOff");
let tls_info = new Array("","Before","After","SN","KeyLength");
let tls_info1 = new Array("","Nation","State","Place","Oragnize","Uint","Name","Mail");
let mqtt_cfg = new Array("","En","Addr","Port","Path","Id","Usr","Psd","Keep","Qos","State");
let ip_mode = new Array("Ipv4","Ipv6");
let ip_addr = new Array("En","Mode","Addr","Mask","Gateway","Prefix","Dns","","","","Card","Mac");
var encrpty_name = new Array("","Encrptyen","","","","","","","","","","AESmode","AESfilling","AESlength","AESkey","AESoffset","","","","","",
"RSAlength","RSAfilling","RSAkey","","","","","","","","SM4mode","SM4filling","SM4key","SM4offset");
let web_service = new Array("","HttpEn","HttpPort","Redirect","HttpsEn","HttpsPort");
let smtp_name = new Array("","SmtpEn","SmtpServer","SmtpSendUsr","SmtpSendPsd","SmtpRecvUsr","SmtpPort","SmtpMode","SmtpState");
let white_list = new Array("","WlCtrlEn","WlMac1","WlMac2","WlIp1","WlIp2");
let ntp_info = new Array("","DevTime","TimeSetEn","NtpServer","TimeZone");
let ssh_name = new Array("","SshEn","TelnetEn","SshName","SshPsd");
let syslog_name = new Array("","SyslogEn","SyslogServer","SyslogPort");
let url_1;
let group_num  = 8;
let total_data = new Array(3);
let inletA_data = new Array(3);
let inletB_data = new Array(3);
let inletC_data = new Array(3);
for(let i = 0;i<3;i++){
  total_data[i] = new Array(3);
  inletA_data[i] = new Array(3);
  inletB_data[i] = new Array(3);
  inletC_data[i] = new Array(3);
  for(let j = 0;j< 50;j++){
    total_data[i][j] = 0;
    inletA_data[i][j] = 0;
    inletB_data[i][j] = 0;
    inletC_data[i][j] = 0;
  }
}
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
  let type = 0,topic = 0,subtopic = 0,num = 0;
  ws.onclose = function(){};
  ws.onmessage = function(evt) {
    let addr_  = parseInt(JSON.parse(evt.data).result[0]);
    type = parseInt(JSON.parse(evt.data).result[1]);
    topic = parseInt(JSON.parse(evt.data).result[2]);
    subtopic = parseInt(JSON.parse(evt.data).result[3]);
    num = parseInt(JSON.parse(evt.data).result[4]);
    switch(type)
    {
      case 0:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 1:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_ +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 2:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_ +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 3:
        if(topic == 1){
          sessionStorage.setItem(type_name[type]+ data_type[topic] + alarm_name[subtopic] + addr_ +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
        }else{
          sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_ +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
        }
      break;
      case 4:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_ +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 5:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_ +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 6:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_ +'_'+num, parseInt(JSON.parse(evt.data).result[5])); 
      break;
      case 7:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_ +'_'+num, parseInt(JSON.parse(evt.data).result[5]));
      break;
      case 10:
        sessionStorage.setItem("Output_name"+ addr_ +'_'+topic,JSON.parse(evt.data).result[5]); 
      break;
      case 11:
        sessionStorage.setItem(uut_name[topic]+ addr_ ,JSON.parse(evt.data).result[5]); 
      break;
      case 12:
        if(topic>10){
          sessionStorage.setItem(type_info[topic]+ "Num" + subtopic  +'_'+ addr_ ,JSON.parse(evt.data).result[5]);
        }else{
          sessionStorage.setItem(type_info[topic]+ "Num" + addr_ ,JSON.parse(evt.data).result[5]);
        }
      break;
      case 13:
        sessionStorage.setItem(cfg_name[topic] + addr_, parseInt(JSON.parse(evt.data).result[5]));
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
      case 19:
        sessionStorage.setItem(type_name[type]+ mqtt_cfg[topic], JSON.parse(evt.data).result[5]);
      break;
      case 21:
        sessionStorage.setItem(type_name[type] + topic, JSON.parse(evt.data).result[5]);
      break;
      case 22:
        sessionStorage.setItem(type_name[type]+ info_info[topic] + addr_ +'_'+subtopic, JSON.parse(evt.data).result[5]);
      break;
      case 23:
        sessionStorage.setItem(type_name[type]+ info_info[topic] + addr_ +'_'+subtopic, JSON.parse(evt.data).result[5]);
      break;
      case 24:
        sessionStorage.setItem(type_name[type]+ info_info[topic] + addr_ +'_'+subtopic, JSON.parse(evt.data).result[5]);
      break;
      case 25:
        sessionStorage.setItem(type_name[type]+ addr_ +'_'+topic, JSON.parse(evt.data).result[5]);
      break;
      case 30:
        sessionStorage.setItem(ver_name[topic] + addr_, parseInt(JSON.parse(evt.data).result[5]));
      break;
      case 31:
        sessionStorage.setItem(encrpty_name[topic], JSON.parse(evt.data).result[5]);
      break;
      case 32:
        if(topic < 5){
          sessionStorage.setItem("Tls"+tls_info[topic], JSON.parse(evt.data).result[5]);
        }else if(topic > 10 && topic<30){
          if(topic / 10 == 1){
            sessionStorage.setItem("Theme"+tls_info1[topic % 10], JSON.parse(evt.data).result[5]);
          }else{
            sessionStorage.setItem("Issue"+tls_info1[topic % 10], JSON.parse(evt.data).result[5]);
          }
        }
      break;
      case 33:
        sessionStorage.setItem(white_list[topic], JSON.parse(evt.data).result[5]);
      break;
      case 41:
        sessionStorage.setItem(ip_mode[subtopic]+ip_addr[topic] , JSON.parse(evt.data).result[5]);
      break;
      case 42:
        sessionStorage.setItem(web_service[topic], JSON.parse(evt.data).result[5]);
      break;
      case 43:
        sessionStorage.setItem(ntp_info[topic], JSON.parse(evt.data).result[5]);
      break;
      case 44:
        sessionStorage.setItem(smtp_name[topic], JSON.parse(evt.data).result[5]);
      break;
      case 45:
        sessionStorage.setItem(ssh_name[topic], JSON.parse(evt.data).result[5]);
      break;
      case 46:
        sessionStorage.setItem(syslog_name[topic], JSON.parse(evt.data).result[5]);
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
var hum_num = 2,num_num = 12,cfg_num = 14,uut_num = 5, sub_num = 8;
var total = 0, phase  = 1,loop = 2,output = 3,group = 4,dual = 5,envir = 6,sensor = 7,bit = 10,uut = 11,num =12, cfg = 13,user  = 14,modbus = 15,snmp = 16,rpc_cfg = 17,push = 18,ver_ = 30,tls_ = 32,log = 81;
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
  var j = 2;
  var time1 = setInterval(function(){
    if(j >= 8){
      clearInterval(time1);
    }
    if(j < 8 && j != 6){
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
function read_group_data(addr)
{
  var j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(group_num + 1)){
      clearInterval(time1);
    }
    if(j <= group_num){
      rpc.call('pduReadData',[addr,group,pow_,1,j]);
      rpc.call('pduReadData',[addr,group,energe_,1,j]);
      rpc.call('pduReadData',[addr,group,AVpow_,1,j]);
      rpc.call('pduReadData',[addr,group,pow_,3,j]);
      rpc.call('pduReadData',[addr,group,pow_,2,j]);
      rpc.call('pduReadData',[addr,group,pow_,4,j]);
      rpc.call('pduReadData',[addr,group,pow_,5,j]);
      rpc.call('pduReadData',[addr,group,pow_,6,j]);
      rpc.call('pduReadData',[addr,group,pow_,7,j]);
      rpc.call('pduReadData',[addr,group,pow_,7,j]);
    }
    j++;
  },3);
}
function read_dual_param_data(addr)
{
  var output_num = parseInt(sessionStorage.getItem('OutputNum' + addr));
  var j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(output_num + 1)){
      clearInterval(time1);
    }
    if(j <= output_num){
      rpc.call('pduReadParam',[addr,24,1,j,j]);
      rpc.call('pduReadParam',[addr,24,2,j,j]);
      rpc.call('pduReadParam',[addr,24,3,j,j]);
    }
    j++;
  },1);
}
function read_group_param_data(addr)
{
  var j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(group_num + 1)){
      clearInterval(time1);
    }
    if(j <= group_num){
      rpc.call('pduReadParam',[addr,23,1,j,j]);
      rpc.call('pduReadParam',[addr,23,2,j,j]);
      rpc.call('pduReadParam',[addr,23,3,j,j]);
    }
    j++;
  },1);
}
function read_dual_data(addr)
{
  var output_num = parseInt(sessionStorage.getItem('OutputNum' + addr));
  var j = 1;var i = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(sub_num + 1)){
      clearInterval(time1);
    }
    if(i <= output_num && j <= sub_num){
      rpc.call('pduReadData',[addr,dual,pow_,j,i]);
    }
    i++;
    if(i >= (output_num + 1)){
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
      if(i <= output_num){
        rpc.call('pduReadData',[addr,output,switch_,j,i]);
        rpc.call('pduReadData',[addr,output,cur_,j,i]);
        rpc.call('pduReadData',[addr,output,pow_,j,i]);
        rpc.call('pduReadData',[addr,output,energe_,j,i]);
        rpc.call('pduReadData',[addr,output,pf_,j,i]);
        rpc.call('pduReadData',[addr,output,AVpow_,j,i]);
        rpc.call('pduReadData',[addr,output,reactpow_,j,i]);
      }
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
    if((j < 7 || j > 10) && j <= num_num){
      if(j > 10){
       var loop_num = parseInt(sessionStorage.getItem('LoopNum' + addr));
        for(let i =0;i<loop_num;i++){
          rpc.call('pduReadParam',[addr,num,j,i,0]);
        }
      }else{
        rpc.call('pduReadParam',[addr,num,j,0,0]);
      }
    }
    j++;
  },1);
}
function read_output_param(addr){
  var output_num = parseInt(sessionStorage.getItem('OutputNum' + addr));
  var j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(output_num + 1)){
      clearInterval(time1);
    }
    if(j <= output_num){
      rpc.call('pduReadParam',[addr,22,1,j,j]);
      rpc.call('pduReadParam',[addr,22,2,j,j]);
      rpc.call('pduReadParam',[addr,22,3,j,j]);
    }
    j++;
  },1);
}
function read_modbus_data(addr){
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
function read_rpc_data(addr){
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

function read_snmp_data(addr){
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
function read_http_data(addr){
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
function read_ver_info(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(15)){
      clearInterval(time1);
    }
    if(j != 10 && j < 15){
      rpc.call('pduReadParam',[addr,ver_,j,0,0]);
    }
    j++;
  },1);
}
function read_tls_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(30)){
      clearInterval(time1);
    }
    if(j < 5 || (j>10 && j<18) || (j>20 && j<28)){
      rpc.call('pduReadParam',[addr,tls_,j,0,0]);
    }
    j++;
  },3);
}
function read_group_info(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(group_num +1)){
      clearInterval(time1);
    }
    if(j < group_num +1){
      rpc.call('pduReadParam',[addr,25,j,0,0]);
    }
    j++;
  },3);
}
function read_mqtt_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(10 +1)){
      clearInterval(time1);
    }
    if(j < 10 +1){
      rpc.call('pduReadParam',[addr,19,j,0,0]);
    }
    j++;
  },3);
}
function read_mode_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(10 +1)){
      clearInterval(time1);
    }
    if(j < 10 +1){
      rpc.call('pduReadParam',[addr,19,j,0,0]);
    }
    j++;
  },3);
}

function read_monitoring_data(addr){
  var j = 1;var i = 1;
  var output_num = parseInt(sessionStorage.getItem('OutputNum' + addr));
  var time1 = setInterval(function(){
    if(i >= parseInt(output_num + 1)){
      clearInterval(time1);
    }
    if(i <= output_num){
      rpc.call('pduReadData',[addr,output,pow_,j,i]);
      rpc.call('pduReadData',[addr+1,output,pow_,j,i]);
      rpc.call('pduReadData',[addr,output,AVpow_,j,i]);
      rpc.call('pduReadData',[addr+1,output,AVpow_,j,i]);
      rpc.call('pduReadData',[addr,output,energe_,j,i]);
      rpc.call('pduReadData',[addr+1,output,energe_,j,i]);
      rpc.call('pduReadData',[addr,output,pow_,3,i]);
      rpc.call('pduReadData',[addr+1,output,pow_,3,i]);
      rpc.call('pduReadData',[addr,output,switch_,1,i]);
      rpc.call('pduReadData',[addr+1,output,switch_,1,i]);
      rpc.call('pduReadData',[addr,output,switch_,8,i]);
      rpc.call('pduReadData',[addr+1,output,switch_,8,i]);
    }
    i++;
  },3);
}
function read_ip_addr_data(addr){
  let j = 0,i =0;
  var time1 = setInterval(function(){
    if(j >= parseInt(11 +1)){
      clearInterval(time1);
    }
    if(j < 11 +1 && (j<7 || j>9)){
      rpc.call('pduReadParam',[addr,41,j,i,0]);
    }
    j++;
    if(j>=12 && i < 1){
      j = 0;
      i++;
    }
  },3);
}
function read_encrpty_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(34 +1)){
      clearInterval(time1);
    }
    if(j < 34 +1){
      if(j == 1 || (j>10 && j<16)|| (j>20 && j< 25)||(j>30 && j<35)){
        rpc.call('pduReadParam',[addr,31,j,0,0]);
      }
    }
    j++;
  },3);
}
function read_web_service_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(5 +1)){
      clearInterval(time1);
    }
    if(j < 5 +1){
      rpc.call('pduReadParam',[addr,42,j,0,0]);
    }
    j++;
  },3);
}
function read_smtp_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(8 +1)){
      clearInterval(time1);
    }
    if(j < 8 +1){
      rpc.call('pduReadParam',[addr,44,j,0,0]);
    }
    j++;
  },3);
}
function read_net_ctrl_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(5 +1)){
      clearInterval(time1);
    }
    if(j < 5 +1){
      rpc.call('pduReadParam',[addr,33,j,0,0]);
    }
    j++;
  },3);
}
function read_ntp_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(4 +1)){
      clearInterval(time1);
    }
    if(j < 4 +1){
      rpc.call('pduReadParam',[addr,43,j,0,0]);
    }
    j++;
  },3);
}
function read_ssh_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(4 +1)){
      clearInterval(time1);
    }
    if(j < 4 +1){
      rpc.call('pduReadParam',[addr,45,j,0,0]);
    }
    j++;
  },3);
}
function read_syslog_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(3 +1)){
      clearInterval(time1);
    }
    if(j < 3 +1){
      rpc.call('pduReadParam',[addr,46,j,0,0]);
    }
    j++;
  },3);
}