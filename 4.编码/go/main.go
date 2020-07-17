package main

import (
	"fmt"
	"net/http"
)
var mdb *ContactDB
type MyHandler struct {}
func (mh MyHandler) ServeHTTP(w http.ResponseWriter,r *http.Request){
	if r.URL.Path =="/contact/"{
		_name := r.FormValue("name")  //FormValue(),如果name存在，也是获取第一个值【查源码就知道了】
		_email := r.PostFormValue("email") // PostFormValue()也是同理，name存在是，也是获取第一个值
		_message := r.Form.Get("message")
		print(_email,_name,_message)

		isRecv := mdb.insertData(_name,_email,_message)
		if isRecv==true {
			w.Write([]byte("send success"))
		}else {
			w.Write([]byte("send failed"))
		}
		return //这里的return是要加的，不然下面的代码也会执行了
	}
	if r.URL.Path == "/contact/contactDB"{
		var msg string
		msg = string(mdb.queryMulti())
		fmt.Fprint(w,msg)
		return
	}
	if r.URL.Path == "/world"{
		w.Write([]byte("world page"))
		return
	}
	// 可以继续写自己的路由匹配规则
	print(r.URL.Path)
}
func makeHandler(handlers ...http.HandlerFunc)http.HandlerFunc{
	return func(w http.ResponseWriter, r *http.Request) {
		for _,hander := range handlers{
			hander(w,r)
		}

	}
}

func main() {
	mdb = &ContactDB{}
	mdb.initDB()
	http.Handle("/contact/", MyHandler{})
	if err := http.ListenAndServe(":7997",nil); err!=nil{
		fmt.Println("start http server fail:", err)
	}
}
