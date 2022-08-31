
	var lastMove = 0;
	 // 监听鼠标按下
	document.onmousedown = function(){  
		lastMove = 1;
	}
	//监听鼠标移动
	document.onmousemove = function(){ 
		lastMove = 1;
	}	
	//监听键盘
	document.onkeydown = function () {
		lastMove = 1;
	};	
	
	window.setInterval(function() {				
		if(lastMove == 1)
		{
			monitorMovement();
			lastMove = 0;			
		}	
	}, 10000)
	
	var xmlset;
	function monitorMovement() 
	{	
		$.ajax({
			dataType:"text",
			url:"/monitorMovement?id1="+lastMove+"&",
			async:true,
			cache:false,
			success:function(sta)
			{
				//alert(sta);
				if (sta == "OK") {

				}
				else {
					
					//document.location.href = "/login.html";
				}
			}
		});
	}
	
	
	//清除所有cookie函数
	function clearAllCookie()
	{
		var date=new Date();
		date.setTime(date.getTime()-10000);
		var keys=document.cookie.match(/[^ =;]+(?=\=)/g);
		console.log("username"+keys);
		if(keys)
		{
			for(var i = keys.length; i--;)
				document.cookie=keys[i]+"=0; expire="+date.toGMTString()+"; path=/";
		}
	}		
	//提取cookie中的值
	function getCookie(name)
	{
		var cookieStr = document.cookie;
		if(cookieStr.length > 0)
		{
			var cookieArr = cookieStr.split(";"); //将cookie信息转换成数组
			for (var i=0; i<cookieArr.length; i++)
			{
				var cookieVal = cookieArr[i].split("="); //将每一组cookie(cookie名和值)也转换成数组
				if(cookieVal[0] == name)
				{
					return unescape(cookieVal[1]); //返回需要提取的cookie值
				}
			}
		}
	}		
	function checkCookie()
	{
		//var cookieUser = document.getElementById("cookieUser");
		var userName = getCookie("username");
		if(userName)
		{
			//loaddata();	
		}	
		else
		{
			clearAllCookie();
			document.location.href = "/login.html";
		}
	}