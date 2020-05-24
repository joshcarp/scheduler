package main

import (
    "fmt"
    "io/ioutil"
    "net/http"
    "os"
)
var filenum int
func main() {
	http.HandleFunc("/", uploadFile)
	http.ListenAndServe(":80", nil)
}
func uploadFile(w http.ResponseWriter, r *http.Request) {
	fmt.Println("File Upload Endpoint Hit")
    fmt.Println(r)
	res, _ := ioutil.ReadAll(r.Body)
	ioutil.WriteFile("out.tar", res, os.ModePerm)
    filenum++



}

// curl -H "Content-Type:application/zip" --data-binary @pkg.zip -X POST http://localhost/
