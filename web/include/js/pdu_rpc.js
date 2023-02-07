/*
 *
 *  Created on: 2023年2月1日
 *      Author: Lzy
 */
class JsonRpc {
    static _instance = null;
    constructor() {
        this.rpcid = 0; 
        this.timeOut = 65; // HTTP最小超时时间 
        this.isSetting = false; // 是否在设置模式
        this.root_map = new Map(); // 唯一的Map表，此表会存储所有的数据
        this.ws = this.socket_open(); // 自动建立连接
    }

    // 生成唯一实例
    static build() {
        if (!JsonRpc._instance) {            
            JsonRpc._instance = new JsonRpc()
        }
        return JsonRpc._instance
    }

    // 自动生成URL
    rpc_url() {
        var protocol = "ws";        
        var host = window.location.host;
        var port = window.location.port; if(port) port = ':' + port;
        if(window.location.protocol == "https:") {protocol = "wss"; this.timeOut = 210;}     
        var url = protocol+'://'+host+port+'/websocket'; //alert(url);
        return url;
    }

    
    asyncSleep(delay) {
        return new Promise((resolve) => setTimeout(resolve, delay));
    }

    // 异步延时
    async sleep(ms) {
        //var dateBegin = new Date().getTime();
        await this.asyncSleep(ms);
        //var res = this.timeFn(dateBegin); 
        //console.log("After waiting" + res);
    }

    // 同步延时
    delay(ms) {
         var waitForMillisecond = ms;           
         var endTime = new Date().getTime() + parseInt(waitForMillisecond, 10);
         while(new Date().getTime() < endTime ) {;}     
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
        alert('json rpc websocket close');
    }

    // 打开Websocket
    socket_open() {        
        var url = this.rpc_url();
        var ws = new WebSocket(url);
        ws.onclose =  function (evt) {JsonRpc.socket_close(evt);}; 
        ws.onmessage = function (event) {JsonRpc.socket_recv(event);};
        return ws;
    }

    // WS套接字发送数据
    socket_send(msg) {
        var ret = true;
        if(this.ws.readyState == WebSocket.OPEN){     
            this.ws.send(msg);
        } else ret = false;
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

        //if(this.isSetting == false)  {
            var key = addr+'_'+type+'_'+topic+'_'+sub+'_'+id;
            // console.log(key,value);
            this.root_map.set(key, value);
        //} 

        return true;
    }

    // RPC读写操作接口
    json_rpc_obj(method, params) {
        const id = this.rpcid++;
        const request = {id, method, params};
        console.log(method,params);
        var msg = JSON.stringify(request);
        return this.socket_send(msg);
    }

    // RPC读取接口
    json_rpc_get(method, params) {
        this.isSetting = false;
        return this.json_rpc_obj(method, params);
    }
   
    // RPC设置接口
    json_rpc_set(method, params) {
        this.isSetting = true;
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

   // 计算两个时间差 dateBegin 开始时间
    timeFn(dateBegin) {
        //如果时间格式是正确的，那下面这一步转化时间格式就可以不用了
        var dateEnd = new Date();//获取当前时间
        var dateDiff = dateEnd.getTime() - dateBegin;//时间差的毫秒数
        var dayDiff = Math.floor(dateDiff / (24 * 3600 * 1000));//计算出相差天数
        var leave1=dateDiff%(24*3600*1000)  //计算天数后剩余的毫秒数
        var hours=Math.floor(leave1/(3600*1000))//计算出小时数
        //计算相差分钟数
        var leave2=leave1%(3600*1000)  //计算小时数后剩余的毫秒数
        var minutes=Math.floor(leave2/(60*1000))//计算相差分钟数
        //计算相差秒数
        var leave3=leave2%(60*1000)   //计算分钟数后剩余的毫秒数
        var seconds=Math.round(leave3/1000)
        var leave4=leave3%(60*1000)   //计算分钟数后剩余的毫秒数
        var minseconds=Math.round(leave4/1000)
        var timeFn = "耗时："+dayDiff+"天 "+hours+"小时 "+minutes+" 分钟"+seconds+" 秒"+minseconds+"毫秒";
        return timeFn;
    }
}

//JsonRpc.build();

// PDU整个JSON包操作类
class PduMetaData {
    constructor() {
        this.addr = 0;
        this.rpc = JsonRpc.build();  
        // setTimeout(function(){PduMetaData.meta_workDown()},this.getTimeOut());
    }

    // 设置地址，并更新JSON数据
    setAddr(addr) {
        this.addr = addr;
        this.meta_workDown();
    }

    getAddr() {
        return this.addr;
    }

    getTimeOut() {
        return this.rpc.getTimeOut();
    }

    // 获取JSON包中某个字段具体的值
    meta_value(type, fc, id=null) {
        var key = this.addr+'_'+100+'_'+0+'_'+0+'_'+0;
        var res = this.rpc.json_rpc_value(key);
        if(res != null) {
            res = res.get(type).get(fc);
            if(id != null) res = res.get(id);
        }
        return res;
    }

    // 启动定时刷新JSON包数据
    meta_start() {
        setInterval(function(){PduMetaData.meta_workDown()}, 1940+this.getTimeOut());  
    }

    // 定时器响应函数
   static meta_workDown(x) {
        var method = "pduMetaData"; 
        var params = [x.addr, 100, 0, 0, 0];
        JsonRpc.build().json_rpc_get(method, params);
   }
} //new PduMetaData().meta_start();


// PDU实时数据操作类
class PduDataItem extends PduMetaData{
    constructor() {
        super();  
    }

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
    cfgValue(type, fc, id, addr=0) {
        var key = addr+'_'+type+'_'+fc+'_'+id+'_'+0;
        console.log(key);
        return this.rpc.json_rpc_value(key);
    }

    // 刷新某个配置参数至Map表中
    getCfg(type, fc, id, addr=0) {
        var method = "pduReadParam"; 
        var params = [addr, type, fc, id,0];
        return this.rpc.json_rpc_get(method, params);
    }

    // 修改某个配置参数
    setCfg(type, fc, value, id, addr=0) {
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

    mqttCfg() {
        var fcs = [1,2,3,4,5,6,7,8,9,10,11,20];
        this.getCfgList(21, fcs);
    }

    swVersion() {
        var type = 30;
        this.getCfgFns(type, 0, 8);
        this.getCfgFns(type, 11, 15);
        this.getCfgFns(type, 21, 23);
    }
}

// 日志操作接口
class PduLog extends  PduCfgs{
    constructor() {
        super();
    }

    log_get (fc, sub, cnt=0, id=0) {
        var key = id+'_'+81+'_'+sub+'_'+fc+'_'+cnt;
        return this.rpc.json_rpc_value(key);
    }

    log_fun(fc, sub, cnt=0, id=0) {
        var method = "pduLogFun"; 
        var params = [id, 81, sub, fc, cnt];
        return this.rpc.json_rpc_get(method, params);
    }

    log_hda(addr, fc, start, end) {
        var method = "pduLogHda"; 
        var params = [addr, 82, fc, start, end];
        return this.rpc.json_rpc_get(method, params);
    }

    log_value(addr, fc, start, end) {
        var key = addr+'_'+82+'_'+fc+'_'+start+'_'+end;
        return this.rpc.json_rpc_value(key);
    }
}

// 实例化接口类
class PduCore extends PduCfgs {
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

    // 清除所有数据
    clear() {
        this.rpc.clear();
    }

    // 异步延时
    msleep(ms) { 
        this.rpc.sleep(ms);
    }

    // 同步延时
    mdelay(ms) {
        this.rpc.delay(ms);
    }

    // 计算耗时时间
    timeCnt(dateBegin) {
        this.rpc.timeFn(dateBegin);
    }

    demo() {
        //this.mqttCfg();
        this.swVersion();
    }
} //var obj = PduCore.build(); setTimeout(function(){ obj.demo(); }, obj.getTimeOut()); setTimeout(function(){  var res = obj.cfgValue(30,0); alert(res); }, 2*obj.getTimeOut());



