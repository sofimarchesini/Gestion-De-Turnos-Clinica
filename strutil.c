#define _POSIX_C_SOURCE 200809L 
#include <string.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strutil.h"

#define CAPACIDAD 2

char *substr(const char *str, size_t n){
    char* cadena = malloc(sizeof(char)*n+1);
    if(cadena==NULL) return NULL;

    size_t tam = strlen(str);

    if(n>tam) n=tam;

    if(n==0){
      cadena[n]='\0';
      return cadena;
    }
    
    for(int i = 0;i < n;i++){
        cadena[i]=str[i];
    }
    cadena[n]='\0';

    return cadena;
}

size_t contador_separadores_(const char *str,char sep){
    size_t contador = 0;
    size_t largo = 0;
    for (size_t i = 0;str[i];i++){
        largo++;
        if (str[i]==sep){
            contador++;
        }
    }
    return contador;
}

char **split(const char *str, char sep){
    
    size_t contador_separadores= contador_separadores_(str,sep);
    
    char **strv = malloc(sizeof(char*)*(contador_separadores+CAPACIDAD));
    if (strv==NULL) return NULL;
    
    contador_separadores+=2;
    size_t largo_palabra = 0;
    size_t largo_strv =0;
    size_t i=0;

    while(str[i]!= '\0'){
        if(str[i]==sep){
            strv[largo_strv]=strndup(str+i-largo_palabra,largo_palabra);
            largo_strv++;
            largo_palabra= 0;
        }
        else{
            largo_palabra++;
        }
        i++;
    }
    if(str[i]!=sep || sep=='\0'){
      strv[largo_strv] = strndup(str+i-largo_palabra,largo_palabra);
      largo_strv++;
    }
    strv[largo_strv]='\0';
    return strv;
}


char* join(char** strv, char sep){

    size_t largo_elementos_strv = 0;
    size_t largo_strv=0;
    
    if(!strv[0]) {
      char *cadena = malloc(sizeof(char)*CAPACIDAD);
      sprintf(cadena,"%c",'\0');
      return cadena;
    }

    while(strv[largo_strv]!=NULL){
      largo_elementos_strv+=strlen(strv[largo_strv]);
      largo_strv++;
    }
    
    char* cadena=malloc(sizeof(char)*(largo_elementos_strv+largo_strv));
    if(cadena==NULL) return NULL;
  
    int pos_cadena=0;
    
    for(size_t pos_strv=0; pos_strv < largo_strv;pos_strv++) {
      size_t largo = strlen(strv[pos_strv]);
      for(size_t pos_palabra=0;pos_palabra< largo;pos_palabra++){
        cadena[pos_cadena] = strv[pos_strv][pos_palabra];
        pos_cadena++;
      }
      if(largo_strv-1==pos_strv) break;
      if (sep){
        cadena[pos_cadena]=sep;
        pos_cadena++;
      }
    }
    cadena[pos_cadena]='\0';
    
    return cadena;
}

void free_strv(char *strv[]){
	for(int i= 0; strv[i];i++){
		free(strv[i]);
	}
	free(strv);
}
