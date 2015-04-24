#include <msp430g2553.h>
#include "displaynokia.h"


void menu0()
{
	acess_linha(1);
	acess_coluna(1);

	printLCD("Escolha 1 opcao");
	acess_linha(3);
	acess_coluna(10);
	printLCD("Conf. Relogio");
	acess_linha(4);
	acess_coluna(10);
	printLCD("Conf. Doses");
	acess_linha(5);
	acess_coluna(10);
	printLCD("Exit");
}


//Relógio///////////////////////////////////////
void relogio_disp(volatile unsigned int Hora_1, volatile unsigned int Hora_0, volatile unsigned int Minuto_1, volatile unsigned int Minuto_0)
{
	acess_disp(29,3);
	escreva_caracter(0x30+Hora_1);
	escreva_caracter(0x30+Hora_0);
	escreva_caracter(':');
	escreva_caracter(0x30+Minuto_1);
	escreva_caracter(0x30+Minuto_0);
}

void setando_rel(volatile unsigned char digito, volatile unsigned int Hora_1, volatile unsigned int Hora_0, volatile unsigned int Minuto_1, volatile unsigned int Minuto_0)
{
	relogio_disp(Hora_1, Hora_0, Minuto_1, Minuto_0);
	if (digito<2)
		acess_disp(29+5*(4-digito),3);
	else
		acess_disp(29+5*(3-digito),3);
	escreva_caracter(' ');



}

void configura_rel(volatile unsigned char Frase[], volatile unsigned int Hora_1, volatile unsigned int Hora_0, volatile unsigned int Minuto_1, volatile unsigned int Minuto_0)
{
	char digito = 0;
	char valor = 0;
	acess_disp(1,1);
	printLCD(Frase);
	while(digito<4)
	{
		if(digito == 0)
			valor = Minuto_0;
		else if(digito == 1)
			valor = Minuto_1;
		else if(digito == 2)
			valor = Hora_0;
		else if(digito == 3)
			valor = Hora_1;

		while(!CLICA_OK)
		{
			if(digito == 0)
			{
				if(valor>9)
					valor = 0;
				Minuto_0 = valor;
			}

			else if(digito == 1)
			{
				if(valor>5)
					valor = 0;
				Minuto_1 = valor;
			}
			else if(digito == 2)
			{
				if((Hora_1<2)&&(valor>9))
					valor=0;
				else if((Hora_1==2)&&(valor>3))
					valor=0;
				Hora_0 = valor;
			}

			else if(digito == 3)
			{
				Hora_1 = valor;
				if((Hora_0>3)&&(Hora_1==2))
				{
					Hora_0=0;
					Hora_1=0;
					setando_rel(digito-1, Hora_1, Hora_0, Minuto_1, Minuto_0);
					setando_rel(digito, Hora_1, Hora_0, Minuto_1, Minuto_0);
					valor=0;
				}
				else if(Hora_1>2)
				{
					Hora_1=0;
					valor=0;
				}

			}
			setando_rel(digito, Hora_1, Hora_0, Minuto_1, Minuto_0);
			if(CLICA_SELECT)
				valor++;

			acess_linha(6);
		}
		digito++;
		atraso(8000);


	}
	limpa_linha(1);
	limpa_linha(3);
}

//Porte do Cachorro///////////////////////////////////////
void sel_porte(volatile unsigned char selecao)
{
	switch (selecao)
	{
	case 0:
		acess_disp(1,3);
		printLCD("[1]____________");
		acess_disp(1,4);
		printLCD("Filhote: 1 Xic.");
		break;
	case 1:
		acess_disp(1,3);
		printLCD("[2]____________");
		acess_disp(1,4);
		printLCD("Pequeno: 2 Xic.");
		break;
	case 2:
		acess_disp(1,3);
		printLCD("[3]____________");
		acess_disp(1,4);
		printLCD("Medio: 3 Xic.  ");
		break;
	case 3:
		acess_disp(1,3);
		printLCD("[4]____________");
		acess_disp(1,4);
		printLCD("Grande: 4 Xic. ");
		break;
	case 4:
		acess_disp(1,3);
		printLCD("[5]____________");
		acess_disp(1,4);
		printLCD("Gigante: 5 Xic.");
		break;
	default : printLCD("ERRO");
	}
}
void menu_port()
{
	volatile unsigned char cont=Porte;
	acess_disp(1,1);
	printLCD("Porte Canino:");
	sel_porte(cont);
	while(!CLICA_OK)
	{
		if(CLICA_SELECT)
		{
			cont++;
			atraso(7000);
			if(cont>4)
				cont = 0;
			sel_porte(cont);
		}


	}
	Porte = cont;
	Doses = cont;
	limpa_linha(1);
	limpa_linha(3);
	limpa_linha(4);
}

/// Ajunte hora da ração

//Relógio///////////////////////////////////////
/*void relogio_racao()
{
	acess_disp(29,3);
	escreva_caracter(0x30+RefHora1);
	escreva_caracter(0x30+RefHora0);
	escreva_caracter(':');
	escreva_caracter(0x30+RefMin1);
	escreva_caracter(0x30+RefMin0);
}

void setando_rel_racao(volatile unsigned char digito)
{
	relogio_racao();
	if (digito<2)
		acess_disp(29+5*(4-digito),3);
	else
		acess_disp(29+5*(3-digito),3);
	escreva_caracter(' ');

}

void configura_rel_racao(volatile unsigned int Hora_1, volatile unsigned int Hora_0, volatile unsigned int Minuto_1, volatile unsigned int Minuto_0)
{
	char digito = 0;
	char valor = 0;
	acess_disp(1,1);
	printLCD("Hora da refeicao");
	while(digito<4)
	{
		if(digito == 0)
			valor = RefMin0;
		else if(digito == 1)
			valor = RefMin1;
		else if(digito == 2)
			valor = Hora0;
		else if(digito == 3)
			valor = Hora1;

		while(!CLICA_OK)
		{
			if(digito == 0)
			{
				if(valor>9)
					valor = 0;
				Minuto0 = valor;
			}

			else if(digito == 1)
			{
				if(valor>5)
					valor = 0;
				Minuto1 = valor;
			}
			else if(digito == 2)
			{
				if((Hora1<2)&&(valor>9))
					valor=0;
				else if((Hora1==2)&&(valor>3))
					valor=0;
				Hora0 = valor;
			}

			else if(digito == 3)
			{
				Hora1 = valor;
				if((Hora0>3)&&(Hora1==2))
				{
					Hora0=0;
					Hora1=0;
					setando_rel(digito-1);
					setando_rel(digito);
					valor=0;
				}
				else if(Hora1>2)
				{
					Hora1=0;
					valor=0;
				}

			}
			setando_rel(digito);
			if(CLICA_SELECT)
				valor++;

			acess_linha(6);
		}
		digito++;
		atraso(8000);


	}
	limpa_linha(1);
	limpa_linha(3);
}*/


///////////menu principal/////////////////

void menu_principal(volatile unsigned char n)
{
	limpa_linha(1);
	limpa_linha(3);
	limpa_linha(4);
	limpa_linha(5);
	switch (n)
	{
	case 0 :
		acess_disp(1,2);
		configura_rel("Ajuste de Hora", Hora1, Hora0, Minuto1, Minuto0);
		delay_ms(500);
		break;
	case 1:
		acess_disp(1,2);
		menu_port();
		delay_ms(500);
		break;
	case 2:
		acess_disp(1,2);
		configura_rel("H. da Refeicao", RefHora1, RefHora0, RefMin1, RefMin0);
		delay_ms(500);
		break;
	default :
		acess_disp(1,2);
		printLCD("NHENHENHE");
		delay_ms(5000);
		limpa_tudo();
		break;
	}
}


void selecao(volatile unsigned char n)
{
	if(n==0)
	{
		acess_disp(1,4);
		escreva_caracter(' ');
		acess_disp(1,5);
		escreva_caracter(' ');
		acess_disp(1,3);
		escreva_caracter('>');
	}
	else if(n==1)
	{
		acess_disp(1,3);
		escreva_caracter(' ');
		acess_disp(1,5);
		escreva_caracter(' ');
		acess_disp(1,4);
		escreva_caracter('>');
	}
	else if(n==2)
	{
		acess_disp(1,3);
		escreva_caracter(' ');
		acess_disp(1,4);
		escreva_caracter(' ');
		acess_disp(1,5);
		escreva_caracter('>');
	}

}


void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    volatile char cont;
    volatile char n=0x00;
    config_clock();

    P1SEL = P1SEL2 = 0;
    P1REN|= BOTAO_SELECT+BOTAO_OK;
    P1OUT = 0x00;
    P1DIR = DATA+SCLOCK+DTCOM+SCEN;

    delay_ms(100);
    P1OUT &= ~SCEN;



    envia_serial(0x21, 0, 0);
    envia_serial(0xBF, 0, 0);//BF VOP
    envia_serial(0x06, 0, 0);//0X06 TEMP 20
    envia_serial(0x13, 0, 0);//0X13 BIAS 0C
    envia_serial(0x20, 0, 0);//0X20 FUNCTION SET -> MODO DE ENTRADA
    envia_serial(0x0C, 0, 0);//0X48 BIAS

/*
      envia_serial(0x21, 0, 0);
    envia_serial(0xBF, 0, 0);//BF VOP
    envia_serial(0x06, 0, 0);//0X06 TEMP
    envia_serial(0x13, 0, 0);//0X13 BIAS
    //envia_serial(0x20, 0, 0);//0X20 FUNCTION SET -> MODO DE ENTRADA
    //envia_serial(0x48, 0, 0);//0X48 BIAS
  */

    limpa_tudo();
    acess_linha(1);
	acess_coluna(1);
	menu0();
   /* printLCD("<<Ze Alberto>>");
    delay_ms(10000);
    limpa_linha(1);
    inverte_disp();
*/

    //envia_serial(0x20, 0, 0);//0X20 FUNCTION SET -> MODO DE ENTRADA
    //envia_serial(0x0C, 0, 0);//0X48 BIAS
   	for(;;)
    	{
   		if(CLICA_SELECT)
   		{
   			n++;
   			delay_ms(100);
   			//delay_ms(5000);

   			//printLCD("Sim ");
   			if (n>2)
   				n=0;

   		}
   		selecao(n);

   		if(CLICA_OK)
   		{
   			menu_principal(n);
   			menu0();
   			delay_ms(100);
   		}
   		//printLCD("Nao ");


   		/*for(cont=0x30; cont<0x3A;cont++)
   		{
   			acess_linha(0x02);
   			acess_coluna(42);
   			escreva_caracter(cont);
   			//limapa_disp();
   			//delay_ms(10000);

   		}*/
    }


}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TimerA0(void)
{
	TACTL &= ~TAIFG;
	LPM0_EXIT;
}
