package main

import (
	"database/sql"
	"encoding/json"
	"fmt"
	_ "github.com/go-sql-driver/mysql"
	"time"
)
const (
	USERNAME = "root"
	PASSWORD = "2427072749"
	NETWORK  = "tcp"
	SERVER   = "localhost"
	PORT     = 3306
	DATABASE = "graduation"
)
type Message struct {
	Name sql.NullString  `db:"name"`  //由于在mysql的users表中name没有设置为NOT NULL,所以name可能为null,在查询过程中会返回nil，如果是string类型则无法接收nil,但sql.NullString则可以接收nil值
	Email sql.NullString `db:"email"`
	Text sql.NullString `db:"text"`
	id int `db:"id"`
}

type ContactDB struct {
	DB *sql.DB
}


func (mydb *ContactDB)initDB() {
	dsn := fmt.Sprintf("%s:%s@%s(%s:%d)/%s",USERNAME,PASSWORD,NETWORK,SERVER,PORT,DATABASE)
	var err error
	mydb.DB,err = sql.Open("mysql",dsn)
	if err != nil{
		fmt.Printf("Open mysql failed,err:%v\n",err)
	}
	mydb.DB.SetConnMaxLifetime(100*time.Second)  //最大连接周期，超过时间的连接就close
	mydb.DB.SetMaxOpenConns(100)//设置最大连接数
	mydb.DB.SetMaxIdleConns(16) //设置闲置连接数
}
func (mydb *ContactDB)queryOne(name string){
	msg := new(Message)
	row := mydb.DB.QueryRow("select * from contact where name=?",name)
	//row.scan中的字段必须是按照数据库存入字段的顺序，否则报错
	if err :=row.Scan(&msg.Name,&msg.Email,&msg.Text,&msg.id); err != nil{
		fmt.Printf("scan failed, err:%v",err)
		return
	}

	fmt.Println(*msg)
}
//查询多行
func (mydb *ContactDB)queryMulti(){
	msg := new(Message)
	rows, err := mydb.DB.Query("select * from contact")
	defer func() {
		if rows != nil {
			rows.Close()
		}
	}()
	if err != nil {
		fmt.Printf("Query failed,err:%v", err)
		return
	}
	for rows.Next() {
		err = rows.Scan(&msg.Name, &msg.Email, &msg.Text,&msg.id)
		if err != nil {
			fmt.Printf("Scan failed,err:%v", err)
			return
		}
		fmt.Println(*msg)
		data,_ := json.Marshal(msg)
		fmt.Print(string(data))

	}

}

//插入数据
func (mydb *ContactDB)insertData(_name string,_email string, _text string) bool{
	result,err := mydb.DB.Exec("insert INTO contact(name,email,text) values(?,?,?)",_name,_email,_text)
	if err != nil{
		fmt.Printf("Insert failed,err:%v",err)
		return false
	}
	lastInsertID,err := result.LastInsertId()
	if err != nil {
		fmt.Printf("Get lastInsertID failed,err:%v",err)
		return false
	}
	fmt.Println("LastInsertID:",lastInsertID)
	rowsaffected,err := result.RowsAffected()
	if err != nil {
		fmt.Printf("Get RowsAffected failed,err:%v",err)
		return false
	}
	fmt.Println("RowsAffected:",rowsaffected)
	return true
}

//更新数据
func (mydb *ContactDB)updateDataName(_name string,_id int){
	result,err := mydb.DB.Exec("UPDATE users set name=? where id=?",_name,_id)
	if err != nil{
		fmt.Printf("Insert failed,err:%v",err)
		return
	}
	rowsaffected,err := result.RowsAffected()
	if err != nil {
		fmt.Printf("Get RowsAffected failed,err:%v",err)
		return
	}
	fmt.Println("RowsAffected:",rowsaffected)
}
func (mydb *ContactDB)updateDataEmail(_email string,_id int){
	result,err := mydb.DB.Exec("UPDATE users set email=? where id=?",_email,_id)
	if err != nil{
		fmt.Printf("Insert failed,err:%v",err)
		return
	}
	rowsaffected,err := result.RowsAffected()
	if err != nil {
		fmt.Printf("Get RowsAffected failed,err:%v",err)
		return
	}
	fmt.Println("RowsAffected:",rowsaffected)
}
func (mydb *ContactDB)updateDataText(_text string,_id int){
	result,err := mydb.DB.Exec("UPDATE users set text=? where id=?",_text,_id)
	if err != nil{
		fmt.Printf("Insert failed,err:%v",err)
		return
	}
	rowsaffected,err := result.RowsAffected()
	if err != nil {
		fmt.Printf("Get RowsAffected failed,err:%v",err)
		return
	}
	fmt.Println("RowsAffected:",rowsaffected)
}
//删除数据
func (mydb *ContactDB)deleteData(_id int){
	result,err := mydb.DB.Exec("delete from users where id=?",_id)
	if err != nil{
		fmt.Printf("Insert failed,err:%v",err)
		return
	}
	rowsaffected,err := result.RowsAffected()
	if err != nil {
		fmt.Printf("Get RowsAffected failed,err:%v",err)
		return
	}
	fmt.Println("RowsAffected:",rowsaffected)
}

//func main() {
//	mdb := &ContactDB{}
//	mdb.initDB()
//	mdb.queryMulti()
//
//}