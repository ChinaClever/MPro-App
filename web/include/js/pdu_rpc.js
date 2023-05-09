/*
 *
 *  Created on: 2023年2月1日
 *      Author: Lzy
 */
class JsonRpc {
    static _errCnt = 0;
    static _instance = null;

    constructor() {
        this.rpcid = 0; 
        this.uuid = ' ';
        this.timeOut = 65; // HTTP最小超时时间 
        this.isSetting = false; // 是否在设置模式
        this.root_map = this.rpc_map(); // 唯一的Map表，此表会存储所有的数据
        this.ws = this.socket_open(); // 自动建立连接
    }

    // 生成唯一实例
    static build() {
        if (!JsonRpc._instance) {            
            JsonRpc._instance = new JsonRpc()
        }
        return JsonRpc._instance
    }

    rpc_map() {
        var map = new Map();
        var sessionStorage = window.sessionStorage;
        var value = sessionStorage.getItem("root_map");
        if(value != null) {
            let json = JSON.parse(value);
            let arr = Object.entries(json);
            map = new Map(arr);
        }   
        value = sessionStorage.getItem('uuid');
        if(value != null)  this.uuid = value;
        return map;
    }


    // 自动生成URL
    rpc_url() {
        var protocol = "ws";        
        var host = window.location.host;
        //var port = window.location.port; if(port) port = ':' + port;
        if(window.location.protocol == "https:") {protocol = "wss"; this.timeOut = 210;}     
        var url = protocol+'://'+host+'/websocket'; //alert(url);
        return url;
    }

    getRootMap() {
        return this.root_map;
    }
    
    // 获取至少所需的连接时间
    getTimeOut() {
        return this.timeOut;
    }

    // 清空表
    clear() {
        this.root_map.clear();
    }

    static socket_close(evt) {
        if(JsonRpc._errCnt++ > 10) alert('json rpc websocket close');
    }    

    static socket_error(evt) {
        if(JsonRpc._errCnt++ > 10) alert('json rpc websocket error');
    }  

    // 打开Websocket
    socket_open() {        
        var url = this.rpc_url();
        var ws = new WebSocket(url); JsonRpc._errCnt = 0;
        ws.onclose = function (evt) {JsonRpc.socket_close(evt);};    
        ws.onerror = function (evt) {JsonRpc.socket_error(evt);};   
        ws.onmessage = function (event) {JsonRpc.socket_recv(event);};
        ws.onopen = function () {JsonRpc.socket_req();};
        return ws;
    }

    static socket_reqSend(method, params) {
        var obj = JsonRpc.build(); var map = obj.getRootMap();
        var key = params[0]+'_'+params[1]+'_'+params[2]+'_'+params[3]+'_'+params[4];
        if(!map.has(key)) obj.json_rpc_get(method, params);
    }
    
    static socket_req(){
        var method = "pduMetaData"; 
        var params = [0, 100, 0, 0, 0];   
        obj.json_rpc_get(method, params);     
        //JsonRpc.socket_reqSend(method, params);

        method = "pduReadParam"; 
        params = [0, 13, 10, 0, 0];
        JsonRpc.socket_reqSend(method, params);
        params = [0, 42, 3, 0, 0];
        JsonRpc.socket_reqSend(method, params);
        params = [0, 14, 1, 0, 0];
        JsonRpc.socket_reqSend(method, params);
        params = [0, 14, 3, 0, 0];
        JsonRpc.socket_reqSend(method, params);
    }

    // WS套接字发送数据
    socket_send(msg) {
        var ret = true;
        if(this.ws.readyState == WebSocket.OPEN){
            this.ws.send(msg);
        } else if(JsonRpc._errCnt++ > 2) {
             ret = false;
             this.ws.close();
             this.ws = this.socket_open(); 
        }
        return ret;
    }

    // WS套接字发送数据
    static socket_recv(evt) {
        //alert(evt.data);
        var data = JSON.parse(evt.data).result;
        if(data) JsonRpc.build().json_rpc_recv(data);
    }

    // RPC接收数据并更新至Map表中
    json_rpc_recv(data) {
        var addr  = data[0];
        var type = data[1];
        var topic = data[2];
        var sub =data[3];
        var id = data[4];
        var value = data[5];
       
        if(-1 == parseInt(value)) {         
            this.ws.close();   
            window.sessionStorage.setItem('uuid', ' ');
            var url = window.location.protocol+"//";            
            url += window.location.host;
            window.location.replace(url);
            //alert(value);alert(url);
        }

        var sessionStorage = window.sessionStorage;
        if((14 == parseInt(type)) && (11 == parseInt(topic))) {
            if(1 == parseInt(value[0])) {   
                var host = window.location.host;
                sessionStorage.setItem('host', host);
                this.uuid = value.slice(3); value = 1;
                sessionStorage.setItem('uuid', this.uuid);
                // alert(value); alert(this.uuid);
            } 
        }

        if((0 == addr) && (13 == type) && (10 == topic)) {
            sessionStorage.setItem('language', value);
        }

        var key = addr+'_'+type+'_'+topic+'_'+sub+'_'+id;
        this.root_map.set(key, value);
        const json = Object.fromEntries(this.root_map);        
        sessionStorage.setItem('root_map',JSON.stringify(json));

        return true;
    }

    // RPC读写操作接口
    json_rpc_obj(method, params) {
        const id = this.rpcid++;
        const request = {id, method, params};
        var msg = JSON.stringify(request);
        return this.socket_send(msg);
    }

    // RPC读取接口
    json_rpc_get(method, params) {
        this.isSetting = false; params.push(0);
        params.push(this.uuid);
        return this.json_rpc_obj(method, params);
    }
   
    // RPC设置接口
    json_rpc_set(method, params) {
        this.isSetting = true;
        params.push(this.uuid);
        return this.json_rpc_obj(method, params);
    }

    // 获取Map中的值
    json_rpc_value(key) {
        var res = null;
        if(this.root_map.has(key)) {
            res = this.root_map.get(key);
        }
        return res;        
    }
} //JsonRpc.build();

// PDU整个JSON包操作类
class PduMetaData {
    static m_addr = 0;
    constructor() {
        this.rpc = JsonRpc.build();
        this.addr = PduMetaData.m_addr;
        setTimeout(function(){PduMetaData.meta_workDown()}, this.getTimeOut());
    }    

    // 设置地址，并更新JSON数据
    setAddr(addr) {
        this.addr = addr;
        PduMetaData.m_addr = addr;
        PduMetaData.meta_workDown();
    }

    getAddr() {
        return this.addr;
    }
 
    getTimeOut() {
        return this.rpc.getTimeOut();
    }

    // 获取JSON包中某个字段具体的值    
    meta_value(addr=PduMetaData.m_addr) {
        var key = parseInt(addr)+'_'+100+'_'+0+'_'+0+'_'+0;
        var res = this.rpc.json_rpc_value(key);        
        if(res == null) {
            key = 0+'_'+100+'_'+0+'_'+0+'_'+0;
            res = this.rpc.json_rpc_value(key);
        }

        return res;
    }

     meta_Json_value(type, fc, id=null) {
        var res = meta_value();
        if(res) {
             res = res.get(type).get(fc);
             if(id != null) res = res.get(id);
        }
        return res;
    }


    // 启动定时刷新JSON包数据
    meta_start(addr=PduMetaData.m_addr) {
        this.addr = addr;
        setInterval(function(){PduMetaData.meta_workDown()}, 1940+this.getTimeOut());  
    }

     // 定时器响应函数
    static meta_workDown(addr=PduMetaData.m_addr) {
        var method = "pduMetaData"; 
        var params = [parseInt(addr), 100, 0, 0, 0];
        JsonRpc.build().json_rpc_get(method, params);
   }  
} //new PduMetaData().meta_start();


// PDU实时数据操作类
class PduDataItem extends PduMetaData{
// class PduDataItem extends PduMetaData{
//     constructor() {`
//         super();  
//     }

    // 根据地址获取某个具体实时数据
    getValueByAddr(addr, type, topic, sub, id) {
        var key = addr+'_'+type+'_'+topic+'_'+sub+'_'+id;
        return this.rpc.json_rpc_value(key);
    }

    // 从Map表中获取某个具体实时数据 地址使用成员变量
    getValue(type, topic, sub, id) {
        return this.getValueByAddr(this.addr, type, topic, sub, id);
    }

    getDataByAddr(addr, type, topic, sub, id) {
        var method = "pduReadData"; 
        var params = [addr, type, topic, sub, id];
        return this.rpc.json_rpc_get(method, params);
    }

    // 刷新某个具体实时数据至Map表中
    getData(type, topic, sub, id) {       
        return this.getDataByAddr(this.addr, type, topic, sub, id);
    }

    setDataByAddr(addr, type, topic, sub, id, value) {
        var method = "pduSetData"; 
        var params = [addr, type, topic, sub, id, value];
        return this.rpc.json_rpc_set(method, params);
    }

    // 修改某个具体实时数
    setData(type, topic, sub, id, value) {
        return this.setDataByAddr(this.addr, type, topic, sub, id, value);
    }

} //var obj = new PduDataItem(); setTimeout(function(){obj.getData(1,2,1,1);}, obj.getTimeOut());


// 单个配置参数操作类
class PduCfgItem extends PduDataItem {
    constructor() {
        super(); 
    }

    // 从Map表中获取某个配置参数
    cfgValue(type, fc, id, addr) {
        var key = addr+'_'+type+'_'+fc+'_'+id+'_'+0;
        return this.rpc.json_rpc_value(key);
    }

    // 刷新某个配置参数至Map表中
    getCfg(type, fc, id, addr) {
        var method = "pduReadParam"; 
        var params = [addr, type, fc, id,0];
        return this.rpc.json_rpc_get(method, params);
    }

    // 修改某个配置参数
    setCfg(type, fc, value, id, addr){
        var method = "pduSetParam"; 
        var params = [addr, type, fc, id, 0, value];
        return this.rpc.json_rpc_set(method, params);
    }

} //var obj = new PduCfgItem(); setTimeout(function(){obj.getCfg(22,1,1);}, obj.getTimeOut());

// 多个配置参数操作类
class PduCfgObj extends PduCfgItem {
    constructor() {
        super();
    }

    // 按ID号刷新多个不连续配置参数至Map表中
    getCfgItems(type, fc, ids, addr=0) {
        for(var it of ids) {
            this.getCfg(type, fc, it, addr);
        } 
    }

    // 按ID号刷新多个连续配置参数至Map表中
    getCfgIds(type, fc, start, end) {
        for(var i=start; i<=end; ++i) {
            this.getCfg(type, fc, i, 0);
        }
    }

     // 按功能码刷新多个不连续配置参数至Map表中
    getCfgList(type, fcs, id=0, addr=0) {
        for(var it of fcs) {
            this.getCfg(type, it, id, addr);
        } 
    }

    // 按功能码刷新多个连续配置参数至Map表中
    getCfgFns(type, start, end) {
        for(var i=start; i<=end; ++i) {
            this.getCfg(type, i, 0, 0);
        }
    }
}

// var obj = new PduCfgObj();
// setTimeout(function(){
//     var fcs = [0, 1, 2, 3, 4, 5];
//     obj.getCfgList(30, fcs);
// }, 250);

// 不同类型配置参数一次读取操作类
class PduCfgs extends PduCfgObj {
    constructor() {
        super();
    }

    loginCfg() {
        this.getCfg(13, 10, 0, 0);
        this.getCfg(14, 1, 0, 0);
        this.getCfg(42, 3, 0, 0);
    }

    mqttCfg() {
        var fcs = [1,2,3,4,5,6,7,8,9,10,11];
        this.getCfgList(19, fcs);
    }

    swVersion() {
        var type = 30;
        this.getCfgFns(type, 0, 8);
        this.getCfgFns(type, 11, 15);
        this.getCfgFns(type, 21, 23);
    }

    dataEncrpty(){
        let fcs = [1,11,12,13,14,15,21,22,23,24,31,32,33,34];
        this.getCfgList(31, fcs);
    }
    tlscert(){
        let fcs = [1,2,3,4,11,12,13,14,15,16,17,21,22,23,24,25,26,27];
        this.getCfgList(32, fcs);
    }
    whiteList(){
        let fcs = [1,2,3,4,5,6,7,8,9];
        this.getCfgList(33, fcs);
    }
    udpPush(){
        for(let i=1;i<5;i++){
            this.getCfgIds(18, i, 1, 4);
        }
        let fcs = [8,9];
        this.getCfgList(18, fcs);
    }
    httpPush()
    {
        let fcs = [11,12,13,14,15,16];
        this.getCfgList(18, fcs);
    }
    rpcCfg(){
        let fcs = [1,2,4,5];
        this.getCfgList(17, fcs);
    }
    ampqCfg() {
        var fcs = [1,2,3,4,5,6,7,8,9,10,11,12];
        this.getCfgList(20, fcs);
    }
    snmpCfg(){
        for(let i=21;i<23;i++){
            this.getCfgIds(16, i, 1, 4);
        }
        let fcs = [1,2,3,11,12,13,14,15];
        this.getCfgList(16, fcs);
    }
    modbusCfg() {
        var fcs = [1,2,3,4,5,6,11,12,13];
        this.getCfgList(15, fcs);
    }
    odbcCfg() {
        var fcs = [1,2,3,4,5,6,7,8,9,10];
        this.getCfgList(21, fcs);
        this.getCfgList(28, fcs);
    }
    OutInGroup(){
        for(let j = 1;j<8 + 1;j++){
            this.getCfg(25,j,0,this.addr);
        }
    }
    baseCfg(){
        var fcs = [10,11];
        for(let i=0;i<8;i++){
            this.getCfgIds(41, i, 0, 1);
        }
        this.getCfgList(41, fcs);
    }
    webCfg(){
        var fcs = [1,2,3,4,5];
        this.getCfgList(42, fcs);
    }
    ntpCfg(){
        var fcs = [1,2,3,4];
        this.getCfgList(43, fcs);
    }
    smtpCfg(){
        var fcs = [1,2,3,4,6,7,8];
        this.getCfgList(44, fcs);
        this.getCfgIds(44, 5, 1, 4);
    }
    sshCfg(){
        var fcs = [1,2,3,4];
        this.getCfgList(45, fcs);
    }
    syslogCfg(){
        var fcs = [1,2,3];
        this.getCfgList(46, fcs);
    }
    radiusCfg(){
        let fcs = [1,2,3,4,5,6];
        let fcs_ = [1,2,3];
        this.getCfgList(48, fcs);
        this.getCfgList(49, fcs_);
    }
    diagnosticsCfg(){
        let fcs = [1,2,3,6,7,8];
        this.getCfgList(93, fcs);
    }
    modesetCfg(){
        let fcs = [3,4,7,8,9,10,21];
        this.getCfgList(13, fcs);
    }
    placesetCfg() {
        var fcs = [1,2,3,4,5,6,7];
        this.getCfgList(11, fcs);
    }
    usrinfoCfg() {
        for(let i = 1;i<6;i++){
            this.getCfgIds(14, i, 0, 4);
        }
    }
    sysinfoCfg() {
        var fcs = [0,1,2,3,4,5,6,7,8,11,12,13,14,21,22,23];
        this.getCfgList(30, fcs);
    }
    fwupdateCfg(slave_num,board_num) {
      for(let i = 1;i<slave_num +1;i++){
        this.getCfg(92,6,4,i);
        this.getCfg(92,6,5,i);
      }
      for(let i = 1;i<board_num +1;i++){
        this.getCfg(92,7,4,i);
        this.getCfg(92,7,5,i);
      }
    }
    logseetingCfg() {
        var fcs = [1,2,3,4,5,6];
        this.getCfgList(47, fcs);
    }
    debugCfg(){
        var fcs = [1,9,10,11,12,13];
        let type_ = [1,1,1,2,2,2,3,6,6,3]; 
        let topic_ = [2,3,4,2,3,4,3,11,12,4]; 
        let fcs_ = [2,5,6,7,4];
        this.getCfgList(13, fcs);
        for(let i = 0;i<type_.length;i++){
            for(let j of fcs_){
                this.getData(type_[i],topic_[i],j,0);
            }
        }
    }
    debugVolData(output_num){
        for(let i = 1;i<output_num +1;i++){
            this.getData(3,2,1,i);
        }
    }
}

// 日志操作接口
class PduLog extends  PduCfgs{
    constructor() {
        super();
    }

    log_get (fc, sub, id, cnt) {
        var key = id+'_'+81+'_'+fc+'_'+sub+'_'+cnt;
        return this.rpc.json_rpc_value(key);
    }

    log_fun(fc, sub, id, cnt) {
        var method = "pduLogFun"; 
        var params = [id, 81,fc, sub, cnt];
        return this.rpc.json_rpc_get(method, params);
    }

    log_hda(addr, fc, start, end) {
        var method = "pduLogHda"; 
        var params = [addr, 82, fc, start, end];
        return this.rpc.json_rpc_get(method, params);
    }

    log_value(addr, fc) {
        var key = addr+'_'+82+'_'+fc;
        return this.rpc.json_rpc_value(key);
    }
}

class PduOta extends  PduLog{
    constructor() {
        super();
    }

    otaValue(fc, id, addr) {
        var key = addr+'_'+92+'_'+fc+'_'+id+'_'+addr;
        return this.rpc.json_rpc_value(key);
    }

    // 刷新某个配置参数至Map表中
    getOta(fc, id, addr) {
        var method = "pduReadParam"; 
        var params = [addr, 92, fc, id, addr];
        return this.rpc.json_rpc_get(method, params);
    }     

}

// 实例化接口类
class PduCore extends PduOta {
    static _instance = null;
    constructor() {
        super();
    }

    static build() {
        if (!PduCore._instance) {
            PduCore._instance = new PduCore()
        }
        return PduCore._instance
    }


    login_check() {
        var sessionStorage = window.sessionStorage;
        var value = sessionStorage.getItem("uuid");
        var res = 0; if(value != null) {
            var host = window.location.host;
            var ip = sessionStorage.getItem('host');    
            if((value.length > 9) && (host == ip)) res = 1;
        }

        if(0 == res) {
            this.logged_out();
            var url = window.location.protocol+"//";            
            url += window.location.host;
            window.location.replace(url);
        }       
        
        return res;
    }

    logged_out() {
        var sessionStorage = window.sessionStorage;
        sessionStorage.setItem('host', ' ');
        sessionStorage.setItem('uuid', ' ');
    }   


    // 清除所有数据
    clear() {
        this.rpc.clear();
    }
} //var obj = PduCore.build(); setTimeout(function(){ obj.demo(); }, obj.getTimeOut()); setTimeout(function(){  var res = obj.cfgValue(30,0); alert(res); }, 2*obj.getTimeOut());



