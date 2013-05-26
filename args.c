/*ejemplo de invocacion:
./args --embed --in=algo -P algo --out=algo --steg=LSB1 -a aes128 -m ecb --pass=hola
./args --embed -i algo -P algo -o algo -s LSB1 -a aes128 -m ecb -p hola */
#include <argtable2.h>
#include "hide.h"

int main(int argc, char **argv){

/*Parametros obligatorios*/
struct arg_lit 	* extract = arg_lit0(NULL,"extract",					"Extraer un archivo");
struct arg_lit 	* embed	= arg_lit0(NULL,"embed",					"Ocultamiento de un archivo");
struct arg_file * in 	= arg_file0("i", "in", "<input>",				"Archivo que se va a ocultar");
struct arg_file * p 	= arg_file1("P", NULL, "<input>",				"Archivo bmp que sera el portador");
struct arg_file * out	= arg_file1("o", "out", "<output>",				"Archivo de salida");
struct arg_rex 	* steg 	= arg_rex1("s", "steg", "LSB1|LSB4|LSBE", NULL, 1,		"Algoritmo de esteganografiado");
/*Parametros opcionales*/
struct arg_rex 	* a 	= arg_rex0("a", NULL, "aes128|aes192|aes256|des", NULL,	1,	"Algoritmo elegido para encriptar");
struct arg_rex * m 	= arg_rex0("m", NULL, "ecb|cfb|ofb|cbc", NULL, 1,		"Modo de cifrado");
struct arg_str 	* pass 	= arg_str0("p", "pass", "<char>",				"Password");

struct arg_end  * end    = arg_end(20);

void * argtable[] = {extract, embed, in, p, out, steg, a, m, pass, end};
const char* progname = "Esteganografia";
int exitcode = 0;
int nerrors;

if (arg_nullcheck(argtable) != 0){
	/* NULL entries were detected, some allocations must have failed */
	printf("%s: insufficient memory\n", progname);
	exitcode = 1;
}

/* Set default argument values prior to parsing */
a->sval[0] = "aes128";
m->sval[0] = "cbc";

nerrors = arg_parse(argc,argv,argtable);

if(extract->count > 0 && embed->count > 0) {
	nerrors = 1;
}

if (nerrors > 0){
	arg_print_errors(stdout, end, progname);
	printf("Parametro invalido. Revise el manual para mas informacion.\n");
	exitcode = 1;
	return exitcode;
}


if(extract->count > 0) {
	exitcode = ext(p->filename[0], out->filename[0], steg->sval[0], a->sval[0], m->sval[0], pass->sval[0]);
}

if(embed->count > 0) {
	exitcode = emb(in->filename[0], p->filename[0], out->filename[0], steg->sval[0], a->sval[0], m->sval[0], pass->sval[0]);
}


arg_freetable(argtable, sizeof(argtable)/sizeof(argtable[0]));

return exitcode;

}
