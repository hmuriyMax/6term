package main

import (
	"fmt"
	"log"
	"net/http"
	"os"
)

var html = "<!DOCTYPE html>" +
	"<form action=\"/create\" method=\"POST\">" +
	"<input type=\"text\" name=\"url\" autofocus placeholder=\"URL\" required>" +
	"<input type=\"submit\" value=\"Войти\"></form>"

func indexHandler(w http.ResponseWriter, r *http.Request) {
	_, err := fmt.Fprint(w, html)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}

func main() {
	port := os.Getenv("PORT")
	if port == "" {
		port = "3000"
	}
	log.Printf("http://localhost:3000")
	mux := http.NewServeMux()
	mux.HandleFunc("/", indexHandler)
	log.Printf("HTTP-server started at localhost:%s", port)
	err := http.ListenAndServe(":"+port, mux)
	if err != nil {
		log.Fatal(err)
	}
}
