package main

import (
    "fmt"
    "io/ioutil"
    "net/http"
    "os"
)

func main() {
	http.HandleFunc("/", uploadFile)
	http.ListenAndServe(":80", nil)
}
func uploadFile(w http.ResponseWriter, r *http.Request) {
	fmt.Println("File Upload Endpoint Hit")
    fmt.Println(r)
	res, _ := ioutil.ReadAll(r.Body)
	ioutil.WriteFile("out.zip", res, os.ModePerm)



}

// curl -H "Content-Type:application/zip" --data-binary @pkg.zip -X POST http://localhost/
