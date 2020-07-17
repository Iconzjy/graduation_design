// GET
function get(url, success, failure)
{
    var xhr = new XMLHttpRequest;
    xhr.open("GET", url);
    xhr.onreadystatechange = function() {
        handleResponse(xhr, success, failure);
    }
    xhr.send();
}

// POST
function post(url, arg, success, failure)
{
    var xhr = new XMLHttpRequest;
    xhr.open("POST", url);
    xhr.setRequestHeader("Content-Length", arg.length);
    xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded;");  //用POST的时候一定要有这句
    xhr.onreadystatechange = function() {
        handleResponse(xhr, success, failure);
    }
    xhr.send(arg);
}



// 处理返回值
function handleResponse(xhr, success, failure){
    if (xhr.readyState == XMLHttpRequest.DONE) {
        if (xhr.status ==  200){
            if (success != null && success != undefined)
            {
                var result = xhr.responseText;
                try{
                    success(result, JSON.parse(result));
                }catch(e){
                    success(result, {});
                }
            }
        }
        else{
            if (failure != null && failure != undefined)
                failure(xhr.responseText, xhr.status);
        }
    }
}

function parseResult(result){
    var rtnVal=new Array()
    result=result.slice(1,result.length)
    var len=result.length
    var resList=0
    while(resList<len){
        if(result.charAt(resList++)==="{"){
            var perVal=""
            while(result.charAt(resList)!=="}"){
                perVal+=result.charAt(resList++)
            }
            resList++//因为{}和{}中间还有个“，”
            rtnVal.push(parsePerVale(perVal))
        }
    }
//    console.debug(rtnVal[0])
    return rtnVal
}

function parsePerVale(perVal){
    var len=perVal.length
    var beg=0
    var commaNum=0
    var perRtnVal=new Array()
    while(commaNum<4){
        var w=""
        while(beg<len){
            if(perVal.charAt(beg)===","){
                beg++
                break
            }else{
                w+=perVal.charAt(beg++)
            }
        }
        if(commaNum === 3){
            while(beg<len){
                w+=perVal.charAt(beg++)
            }
        }

        perRtnVal.push(w)
        commaNum++
    }
    return perRtnVal
}
