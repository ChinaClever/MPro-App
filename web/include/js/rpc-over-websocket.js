// JSON-RPC over Websocket implementation
var JSONRPC_TIMEOUT_MS = 1000;
var language = 0;
let verfity = 0;
let ws,pending = {};
let rpcid = 0;
let value_now = 0, val = 0;
let user_name='user_name';
let password = 'password';
let identify = '';
let default_addr = 0;
let slave_addr = 0;
let dual_addr = 0;
let type_info = new Array("","Phase","Loop","Output","Board","Slave","BoardOutput","LoopOutput","","","","LoopStart","LoopEnd");
let type_name = new Array("Total","Phs","Loop","Output","Group","Dual","TH","Sensor","","","Output","Uut","Num","Cfg","User","Modbus","Snmp","Rpc","Push","Mqtt","Amqp","Content","Output","Group","Dual","GroupInfo","GroupSet");
let data_type = new Array("","Sw","Vol","Cur","Pow","Enger","Pf","AVpow","React","EngHis","LineVol","Tmp","Hum","","","","","","","","","Door1","Door2","Water","Smoke");
let data_name = new Array("Size","Val","Rated","Alarm","Max","Min","Vcmin","Vcmax","Enable","MaxVal","MaxTime","HisEn");
let alarm_name = new Array("","State","Mode","Alarm","Seq","Reset","Overrun","Timeout","Enable");
let cfg_name = new Array("Offline","Serial","DevState","DevMode","SlaveAddr","RunTime","Freq","Buz","GroupSwEn","EnergeSwEn","LanguageEn","BreakerEn","Direction","Angle","","","","","","","","JsonLen");
let uut_name = new Array("","RoomName","AddrInfo","DevName","QRCode","DevSN","DevType");
let user_info = new Array("","UserName","Password","Identify","Jurisdiction","OutCtrl","","","","","","Verfity");
let log_info = new Array("","LogNum","LogInfo");
let modbus_info = new Array("","Enable","Addr","Baud","Parity","Data","Stop","","","","","TcpEnable","TcpPort");
let snmp_info = new Array("","V2Enable","","","","","","","","","","V3Enable","Username","Password","Key","Encrpty","","","","","","Trap","");
let rpc_info = new Array("","JsonMode","JsonPort","","XmlMode","XmlPort","");
let push_info = new Array("","UdpEn","UdpAddr","UdpPort","UdpDelay","","","","CtrlMode","Ctrlport","","HttpEn","HttpAddr","HttpTimeout","HttpDelay","RecEncrypt","RecvProt","HttpDelay");
let ver_name = new Array("McName","McVer","McCode","McUsrName","McVerShow","McReiyOnVer","McCompileTime","McReleaseDate","McUpdateDate","","",
"Borad1Ver","Borad2Ver","Borad3Ver","Borad4Ver","","","","","","","CoreVer","MemSize","DiskInfo");
let info_info = new Array("","Name","PowerOn","PowerOff");
let tls_info = new Array("","Before","After","SN","KeyLength");
let tls_info1 = new Array("","Nation","State","Place","Oragnize","Uint","Name","Mail");
let mqtt_cfg = new Array("","En","Addr","Port","Path","Id","Usr","Psd","Keep","Qos","State");
let amqp_cfg = new Array("","En","Addr","Port","Host","Usr","Psd","Swith","Routing","Binding","SslEn","State","Update");
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
let logset_name = new Array("","HistoryEn","EnergeDelay","HistoryDelay","AlarmMaxNum","EventMaxNum","HistoryMaxNum");
let Progress_name = new Array("","","","","","","Slave","Board");
let Progress_info = new Array("","","","","State","Progress");
let net_diagn = new Array("","NetAddr","RequstNum","Ping","Ping1","","Host","Timeout","Router","Router1");
let radius_name = new Array("","RadiusEn","RadiusLocalEn","RadiusServer","RadiusKey","RadiusLocalPort","RadiusPort");
let ldap_name = new Array("","LdapEn","LdapServer","LdapArea");
let odbc_cfg = new Array("","OdbcEn","OdbcServer","OdbcPort","OdbcUsr","OdbcPsd","OdbcName","OdbcKey","OdbcUpdate","OdbcRecord","OdbcState");
let redis_cfg = new Array("","RedisEn","RedisServer","RedisPort","RedisPsd","RedisDbNum","RedisKey","RedisChannel","RedisUpdate","RedisTime","RedisState");
// var pattern = /^.*(?=.{6,16})(?=.*\d)(?=.*[A-Z]{2,})(?=.*[a-z]{2,})(?=.*[!@#$%^&*?\(\)]).*$/;
var pattern = /^.*(?=.{6,16})(?=.*\d)(?=.*[A-Z]{1,})(?=.*[a-z]{1,}).*$/;
// sessionStorage.setItem("dev_addr",0);
if(sessionStorage.getItem("dev_addr") != null){
  addr = parseInt(sessionStorage.getItem("dev_addr"));
}else{
  addr = default_addr;
}
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
  }
  let url = pro + url_ +'/websocket';
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
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_, (JSON.parse(evt.data).result[5])); 
      break;
      case 1:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_ +'_'+num, (JSON.parse(evt.data).result[5])); 
      break;
      case 2:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_ +'_'+num, (JSON.parse(evt.data).result[5])); 
      break;
      case 3:
        if(topic == 1){
          sessionStorage.setItem(type_name[type]+ data_type[topic] + alarm_name[subtopic] + addr_ +'_'+num, (JSON.parse(evt.data).result[5])); 
        }else{
          sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_ +'_'+num, (JSON.parse(evt.data).result[5])); 
        }
      break;
      case 4:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_ +'_'+num, (JSON.parse(evt.data).result[5])); 
      break;
      case 5:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_ +'_'+num, (JSON.parse(evt.data).result[5])); 
      break;
      case 6:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_ +'_'+num, (JSON.parse(evt.data).result[5])); 
      break;
      case 7:
        sessionStorage.setItem(type_name[type]+ data_type[topic] + data_name[subtopic] + addr_ +'_'+num, (JSON.parse(evt.data).result[5]));
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
        }else if(topic == 6 || topic ==7){
          sessionStorage.setItem(type_info[topic]+ "Num" + '_' + subtopic,JSON.parse(evt.data).result[5]);
        }else{
          sessionStorage.setItem(type_info[topic]+ "Num" + addr_ ,JSON.parse(evt.data).result[5]);
        }
      break;
      case 13:
        sessionStorage.setItem(cfg_name[topic] + addr_, (JSON.parse(evt.data).result[5]));
        //console.log(cfg_name[topic] + addr_, (JSON.parse(evt.data).result[5]));
      break;
      case 14:
        if(topic  == 11)
        {
          verfity = 1;
        }
        sessionStorage.setItem(user_info[topic] + subtopic , JSON.parse(evt.data).result[5]);
      break;
      case 15:
        sessionStorage.setItem(type_name[type]+ modbus_info[topic], JSON.parse(evt.data).result[5]);
      break;
      case 16:
        if(topic == 21){
          sessionStorage.setItem(type_name[type]+ snmp_info[topic] + subtopic, JSON.parse(evt.data).result[5]);
        }else{
          sessionStorage.setItem(type_name[type]+ snmp_info[topic], JSON.parse(evt.data).result[5]);
        }
      break;
      case 17:
        sessionStorage.setItem(type_name[type]+ rpc_info[topic], JSON.parse(evt.data).result[5]);
      break;
      case 18:
        if(topic < 5){
          sessionStorage.setItem(type_name[type]+ push_info[topic] + subtopic, JSON.parse(evt.data).result[5]);
        }else{
          sessionStorage.setItem(type_name[type]+ push_info[topic], JSON.parse(evt.data).result[5]);
          // console.log(type_name[type]+ push_info[topic], JSON.parse(evt.data).result[5]);
        }
      break;
      case 19:
        sessionStorage.setItem(type_name[type]+ mqtt_cfg[topic], JSON.parse(evt.data).result[5]);
      break;
      case 20:
        sessionStorage.setItem(type_name[type]+ amqp_cfg[topic], JSON.parse(evt.data).result[5]);
      break;
      case 21:
        sessionStorage.setItem(odbc_cfg[topic], JSON.parse(evt.data).result[5]);
        // console.log(odbc_cfg[topic], JSON.parse(evt.data).result[5]);
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
      case 28:
        sessionStorage.setItem(redis_cfg[topic], JSON.parse(evt.data).result[5]);
      break;
      case 30:
        sessionStorage.setItem(ver_name[topic], JSON.parse(evt.data).result[5]);
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
        if(topic == 5){
          sessionStorage.setItem(smtp_name[topic] + subtopic, JSON.parse(evt.data).result[5]);
        }else{
          sessionStorage.setItem(smtp_name[topic], JSON.parse(evt.data).result[5]);
        }
      break;
      case 45:
        sessionStorage.setItem(ssh_name[topic], JSON.parse(evt.data).result[5]);
      break;
      case 46:
        sessionStorage.setItem(syslog_name[topic], JSON.parse(evt.data).result[5]);
      break;
      case 47:
        sessionStorage.setItem(logset_name[topic], JSON.parse(evt.data).result[5]);
      break;
      case 48:
        sessionStorage.setItem(radius_name[topic], JSON.parse(evt.data).result[5]);
      break;
      case 49:
        sessionStorage.setItem(ldap_name[topic], JSON.parse(evt.data).result[5]);
      break;
      case 51:
      break;
      case 81:
        sessionStorage.setItem(log_info[subtopic] , JSON.parse(evt.data).result[5]);
        console.log(log_info[subtopic] ,JSON.parse(evt.data).result[5]);
      break;
      case 82:
        sessionStorage.setItem("LogData" , JSON.parse(evt.data).result[5]);
        console.log(JSON.parse(evt.data).result[5]);
      break;
      case 92:
        sessionStorage.setItem(Progress_name[topic]+Progress_info[subtopic] +addr_, JSON.parse(evt.data).result[5]);
      break;
      case 93:
        sessionStorage.setItem(net_diagn[topic], JSON.parse(evt.data).result[5]);
        // console.log(net_diagn[topic], JSON.parse(evt.data).result[5]);
      break;
      default:
        break;
    }
  }
  ws.onopen = function(){
      read_user_info();
      read_language();
      read_num_info(addr);
      read_uut_info(default_addr);
      read_cfg_info(default_addr);
  };
  return {
    close:() => ws.close(),
    call:function(method,params){
      const id = rpcid++,request = {id, method, params};
       ws.send(JSON.stringify(request));
      // return new Promise(function(resolve, reject) {
      //   // setTimeout(JSONRPC_TIMEOUT_MS, function() {
      //   //   if (pending[id] === undefined) return;
      //   //   delete (pending[id]);
      //   //   reject();
      //   // });
      //   pending[id] = x => resolve(x);
      // });
    },
  };
}
var rpc;
if(sessionStorage.getItem("CreateConnect") == 1){
  rpc = jsonrpc();
  window.shareSocket = rpc;
  sessionStorage.setItem("CreateConnect", "0");
}else{
  rpc = window.top.shareSocket;
}
var start  = 0;
var hum_num = 2,num_num = 12,cfg_num = 22,uut_num = 6, sub_num = 11;
var total = 0, phase  = 1,loop = 2,output = 3,group = 4,dual = 5,envir = 6,sensor = 7,bit = 10,uut = 11,num =12, cfg = 13,user  = 14,modbus = 15,snmp = 16,rpc_cfg = 17,push = 18,ver_ = 30,tls_ = 32,log = 81;
var switch_ = 1,vol_ = 2,cur_ = 3,pow_ = 4,energe_ = 5,pf_ = 6,AVpow_ = 7,reactpow_ = 8,tmp_ = 11, hum_ = 12, door1_ = 21,door2_ = 22,water_ = 23,smoke_ =24;
var idc_ = 1,room_ = 2;module_ = 3,cabnite_ = 4, loop_ = 5, dev_ = 6;

function read_language(){
  rpc.call('pduReadParam',[0,cfg,10,0,0]);
  rpc.call('pduReadParam',[0,42,3,0,0]);
}
function read_user_info(){
  var j = 1;
  var time1 = setInterval(function(){
    if(j >= 6){
      clearInterval(time1);
    }
    if(j < 6 ){
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
  var j = 1;var i = 0;
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
    if(i <= phase_num ){
      rpc.call('pduReadData',[addr,phase,10,j,i]);
    }
    i++;
    if(i >= (phase_num + 1)){
      i = 0;
      j++;
    }
  },1);
}
function read_loop_data(addr)
{
  var loop_num = parseInt(sessionStorage.getItem('LoopNum' + addr));
  var j = 1;var i = 0;
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
      i = 0;
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
      if(j<14 || j>20){
        rpc.call('pduReadParam',[addr,cfg,j,0,0]);
      }
    }
    j++;
  },1);
}
function read_group_data(addr)
{
  var j = 0;
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
  var j = 1;var i = 0;
  var time1 = setInterval(function(){
    if(j >= parseInt(sub_num + 1)){
      clearInterval(time1);
    }
    if(i <= output_num && j <= sub_num){
      rpc.call('pduReadData',[addr,dual,pow_,j,i]);
      rpc.call('pduReadData',[addr,dual,switch_,j,i]);
    }
    i++;
    if(i >= (output_num + 1)){
      i = 0;
      j++;
    }
  },1);
}
function read_output_data(addr)
{
  var output_num = parseInt(sessionStorage.getItem('OutputNum' + addr));
  var j = 1;var i = 0;
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
        i = 0;
        j++;
      }
  },1);
}
function read_sensor_data(addr)
{
  let j = 1, i = 0;
  let time1 = setInterval(function(){
    if(i >= parseInt(hum_num + 1)){
      clearInterval(time1);
    }
    if(i <= hum_num && j <= sub_num){
      if((j == 1) && i == 1){
        rpc.call('pduReadData',[addr,sensor,door1_,j,i]);
        rpc.call('pduReadData',[addr,sensor,door2_,j,i]);
        rpc.call('pduReadData',[addr,sensor,water_,j,i]);
        rpc.call('pduReadData',[addr,sensor,smoke_,j,i]);
      }
      if(j!= 9 || j!= 10){
        rpc.call('pduReadData',[addr,envir,tmp_,j,i]);
        rpc.call('pduReadData',[addr,envir,hum_,j,i]);
      }
      j++;
      if(j >= parseInt(sub_num + 1)){
        j = 1;
        i++;
      }
    }
  },3);
}
function read_num_info(addr){
  var j = 1;
  var loop_num=0;
  var time1 = setInterval(function(){
    if(j >= parseInt(num_num + 1)){
      clearInterval(time1);
      let board_num = parseInt(sessionStorage.getItem('BoardNum' + addr));
      for(let i = 0;i<board_num;i++){
        rpc.call('pduReadParam',[addr,num,6,i,0]);
      }
      for(let i = 0;i<loop_num;i++){
        rpc.call('pduReadParam',[addr,num,7,i,0]);
      }
    }
    if((j < 8 || j > 10) && j <= num_num){
      if(j > 10){
       loop_num = parseInt(sessionStorage.getItem('LoopNum' + addr));
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
  },3);
}

function read_snmp_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(22)){
      clearInterval(time1);
    }
    if(j <= 21 ){
      if(j == 21){
        for( i = 1;i<5;i++){
          rpc.call('pduReadParam',[addr,snmp,j,i,0]);
        }
      }else if((j>10 && j<16)|| j==1){
        rpc.call('pduReadParam',[addr,snmp,j,0,0]);
      }
    }
    j++;
  },3);
}
function read_log_data(type,name,start,num){
  rpc.call('pduLogFun',[start,log,type,name,num]);
}
function read_push_data(){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(11)){
      clearInterval(time1);
    }
    if(j <= 10 ){
      if(j<5){
        for(let i = 1;i< 5;i++){
          rpc.call('pduReadParam',[addr,push,j,i,0]);
        }
      }else if(j>7){
        rpc.call('pduReadParam',[addr,push,j,0,0]);
      }
    }
    j++;
  },1);


}
function read_http_data(addr){
  let j = 11;
  var time1 = setInterval(function(){
    if(j >= parseInt(17)){
      clearInterval(time1);
    }
    if(j < 17){
      rpc.call('pduReadParam',[addr,push,j,0,0]);
    }
    j++;
  },1);
}
function read_ver_info(addr){
  let j = 0;
  var time1 = setInterval(function(){
    if(j >= parseInt(24)){
      clearInterval(time1);
    }
    if(j < 9 || (j>10 && j < 15)|| (j>20 && j<24)){
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
    if(j >= parseInt(11 +1)){
      clearInterval(time1);
    }
    if(j < 11 +1){
      rpc.call('pduReadParam',[addr,19,j,0,0]);
    }
    j++;
  },5);
}
function read_amqp_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(12 +1)){
      clearInterval(time1);
    }
    if(j < 12 +1){
      rpc.call('pduReadParam',[addr,20,j,0,0]);
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
  var j = 1;var i = 0;
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
      if(j == 5){
        rpc.call('pduReadParam',[addr,44,j,1,0]);
        rpc.call('pduReadParam',[addr,44,j,2,0]);
        rpc.call('pduReadParam',[addr,44,j,3,0]);
        rpc.call('pduReadParam',[addr,44,j,4,0]);
      }else{
        rpc.call('pduReadParam',[addr,44,j,0,0]);
      }
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
function read_log_setting_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(6 +1)){
      clearInterval(time1);
    }
    if(j < 6 +1){
      rpc.call('pduReadParam',[addr,47,j,0,0]);
    }
    j++;
  },3);
}
function read_update_progress_data(){
  let j = 1;
  let slave_num = parseInt(sessionStorage.getItem("SlaveNum" + addr));
  let board_num = parseInt(sessionStorage.getItem("BoardNum" + addr));
  var time1 = setInterval(function(){
    if(j >= parseInt(2)){
      clearInterval(time1);
    }
    if(j < 2){
      for(let i = 1;i<slave_num +1;i++){
        rpc.call('pduReadParam',[i,92,6,4,0]);
        rpc.call('pduReadParam',[i,92,6,5,0]);
      }
      for(let i = 1;i<board_num +1;i++){
        rpc.call('pduReadParam',[i,92,7,4,0]);
        rpc.call('pduReadParam',[i,92,7,5,0]);
      }
    }
    j++;
  },3);
}
function read_net_diagnostics_data(){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(9)){
      clearInterval(time1);
    }
    if(j < 3 || (j>5 && j<8)){
        rpc.call('pduReadParam',[0,93,j,0,0]);
    }
    j++;
  },3);
}
function read_radius_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(7)){
      clearInterval(time1);
    }
    if(j<7){
      rpc.call('pduReadParam',[addr,48,j,0,0]);
    }
    j++;
  },3);
}
function read_ldap_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(4)){
      clearInterval(time1);
    }
    if(j<4){
      rpc.call('pduReadParam',[addr,49,j,0,0]);
    }
    j++;
  },3);
}
function read_odbc_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(10 +1)){
      clearInterval(time1);
    }
    if(j < 10 +1){
      rpc.call('pduReadParam',[addr,21,j,0,0]);
    }
    j++;
  },3);
}
function read_redis_data(addr){
  let j = 1;
  var time1 = setInterval(function(){
    if(j >= parseInt(10 +1)){
      clearInterval(time1);
    }
    if(j < 10 +1){
      rpc.call('pduReadParam',[addr,28,j,0,0]);
    }
    j++;
  },3);
}
function read_history_data(addr,satrt,end,fc){
  rpc.call('pduReadParam',[addr,82,fc,satrt,end]);
}
function read_usrs_data(){
  let j = 1, i = 0;
  let time1 = setInterval(function(){
    if(i >= parseInt(4 + 1)){
      clearInterval(time1);
    }
    rpc.call('pduReadParam',[0,user,j,i,0]);
    j++;
    if(j >= parseInt(5 + 1)){
      j = 1;
      i++;
    }
  },3);
}
function read_sw_data(addr){
  let i = 0;
  var output_num = parseInt(sessionStorage.getItem('OutputNum' + addr));
  let time1 = setInterval(function(){
    if(i >= parseInt(output_num + 1)){
      clearInterval(time1);
    }
    if(i <= output_num){
      rpc.call('pduReadData',[addr,output,switch_,1,i]);
      rpc.call('pduReadData',[addr,output,switch_,2,i]);
      rpc.call('pduReadData',[addr,output,switch_,4,i]);
      rpc.call('pduReadData',[addr,output,switch_,5,i]);
      rpc.call('pduReadData',[addr,output,switch_,8,i]);
    }
    i++;
  },3);
}
function read_debug_data(){
  var j = 1;
    var time1 = setInterval(function(){
      if(j >= parseInt(4 + 1)){
        clearInterval(time1);
      }
      if(j < 5){
        rpc.call('pduReadParam',[addr,num,j,0,0]);
      }
      j++;
    },3);
  setTimeout(function(){
    read_data_frist();
    read_data_second();
    read_data_third();
    read_data_fourth();
    read_data_fifth();
  },300);
}
function read_data_frist(){
  let j =2;
  var time2 = setInterval(function(){
    if(j >= parseInt(7 + 1)){
      clearInterval(time2);
    }
    if((j <= 7 && j != 3)){
      rpc.call('pduReadData',[addr,phase,vol_,j,0]);
      rpc.call('pduReadData',[addr,phase,cur_,j,0]);
      rpc.call('pduReadData',[addr,phase,pow_,j,0]);
      rpc.call('pduReadData',[addr,loop,vol_,j,0]);
      rpc.call('pduReadData',[addr,loop,cur_,j,0]);
      rpc.call('pduReadData',[addr,loop,pow_,j,0]);
      rpc.call('pduReadData',[addr,output,cur_,j,0]);
      rpc.call('pduReadData',[addr,envir,tmp_,j,0]);
      rpc.call('pduReadData',[addr,envir,hum_,j,0]);
    }
    j++;
  },3);
}
function read_data_second(){
  let j = 1;
  var time3 = setInterval(function(){
    let output_num_ = parseInt(sessionStorage.getItem('OutputNum' + addr));
    if(j >= parseInt(output_num_ + 1)){
      clearInterval(time3);
    }
    if(j <= output_num_){
      rpc.call('pduReadData',[addr,output,cur_,2,j]);
      rpc.call('pduReadData',[addr,output,vol_,1,j]);
    }
    j++;
  },3);
}
function read_data_third(){
  var time4 = setInterval(function(){
    rpc.call('pduReadParam',[addr,cfg,1,0,0]);
    rpc.call('pduReadParam',[addr,cfg,10,0,0]);
    rpc.call('pduReadParam',[addr,cfg,11,0,0]);
    rpc.call('pduReadParam',[addr,cfg,13,0,0]);
    rpc.call('pduReadParam',[addr,41,11,0,0]);
    clearInterval(time4);
  },10);
}
function read_data_fourth(){
  let j = 0;
  var time5 = setInterval(function(){
    let loop_num_ = parseInt(sessionStorage.getItem('LoopNum' + addr));
    if(j >= parseInt(loop_num_ + 1)){
      clearInterval(time5);
    }
    if(j <= loop_num_){
      rpc.call('pduReadParam',[addr,num,7,j,0]);
      // console.log(j);
    }
    j++;
  },10);
}
function read_data_fifth(){
  let j = 0;
  var time6 = setInterval(function(){
    let board_num_ = parseInt(sessionStorage.getItem('BoardNum' + addr));
    if(j >= parseInt(board_num_ + 1)){
      clearInterval(time6);
    }
    if(j <= board_num_){
      rpc.call('pduReadParam',[addr,num,6,j,0]);
      // console.log(j);
    }
    j++;
  },10);
}
function change(sel){
  addr = parseInt(sel.value);
  sessionStorage.setItem("dev_addr", parseInt(sel.value));
  var time1 = setTimeout(function(){
    read_num_info(addr);
    read_cfg_info(addr);
    setTimeout(function(){  
      read_output_param(addr);
      read_group_param_data(addr);
      read_group_info(addr);
      read_monitoring_data(addr);
    },500);
  },1500);
}
function ReadDataSend(Addr,Type,Topic,Subtopic,Id,Num){
  if(Id == 0){
    for(let i = 1; i < Num + 1;i++){
      rpc.call('pduReadData',[Addr,Type,Topic,Subtopic,i]);
    }
  }else{
    rpc.call('pduReadData',[Addr,Type,Topic,Subtopic,Id]);
  }
}
function SetDataSend(Addr,Type,Topic,Subtopic,Id,Value,Num){
  if(Id == 0){
    for(let i = 1; i < Num + 1;i++){
      rpc.call('pduSetData',[Addr,Type,Topic,Subtopic,i,Value]);
    }
  }else{
    rpc.call('pduSetData',[Addr,Type,Topic,Subtopic,Id,Value]);
  }
}
function ReadHomeData(){
  var phase_num = parseInt(sessionStorage.getItem('PhaseNum' + addr));
  var loop_num = parseInt(sessionStorage.getItem('LoopNum' + addr));
  ReadDataSend(addr, total, pow_, 1, 1,0);
  ReadDataSend(addr, total, vol_, 1, 1,0);
  ReadDataSend(addr, total, cur_, 1, 1,0);
  ReadDataSend(addr, total, energe_, 1, 1,0);
  ReadDataSend(addr, total, AVpow_, 1, 1,0);
  ReadDataSend(addr, phase, vol_, 1, 0,phase_num);
  ReadDataSend(addr, phase, cur_, 1, 0,phase_num);
  ReadDataSend(addr, phase, cur_, 3, 0,phase_num);
  ReadDataSend(addr, phase, cur_, 4, 0,phase_num);
  ReadDataSend(addr, phase, pf_, 1, 0,phase_num);
  ReadDataSend(addr, phase, pow_, 1, 0,phase_num);
  ReadDataSend(addr, phase, 10, 1, 0,phase_num);
  ReadDataSend(addr, loop, switch_, 1, 0,loop_num);
  ReadDataSend(addr, loop, vol_, 1, 0,loop_num);
  ReadDataSend(addr, loop, cur_, 1, 0,loop_num);
  ReadDataSend(addr, loop, cur_, 4, 0,loop_num);
  ReadDataSend(addr, loop, energe_, 1, 0,loop_num);
  ReadDataSend(addr, loop, pow_, 1, 0,loop_num);
		
  ReadDataSend(addr, envir, tmp_, 1, 1,0);
  ReadDataSend(addr, envir, tmp_, 1, 2,0);
  ReadDataSend(addr, envir, hum_, 1, 1,0);
  ReadDataSend(addr, envir, hum_, 1, 2,0);
  ReadDataSend(addr, sensor, door1_, 1, 1,0);
  ReadDataSend(addr, sensor, door2_, 1, 1,0);
  ReadDataSend(addr, sensor, water_, 1, 1,0);
  ReadDataSend(addr, sensor, smoke_, 1, 1,0);
}