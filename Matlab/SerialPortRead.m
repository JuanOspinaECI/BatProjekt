
%function voltaje=ADC_Serial(muestras) %la funcion recibe el # de muestras que debe tomar
close all;
clear all;
clc;
voltaje=0; %Declara variable en la cual se van a guardar los valores

%Borra datos que se encuentren previos y vuelve a declarar el puerto y la
%velocidad de transmisi�n
delete(instrfind({'port'},{'COM7'})); 
puerto=serial('COM7');
puerto.BaudRate=9600;

figure('Name','Grafica voltaje')
title('GRAFICA VOLTAJE ADC-SERIAL');
xlabel('Numero de Muestras');
ylabel('Voltaje (V)');
grid off;
hold on;

fopen(puerto);%abre el puerto a utilizar
contador=1;
muestras = 10;
cont1 = 1;
cont2 = 1;
while contador<=muestras
    ylim([0 200]); %Limites del eje y
    xlim([0 contador/2 + 2]); %Limites del eje x
    data{contador} = fscanf(puerto,'%d %d');
    if(data{contador}(1) == 1)
        data1(cont1) = data{contador}(2); %Dividir segun ADC y calibracion
        cont1 = cont1 + 1;
    end
    if(data{contador}(1) == 2)
        data2(cont2) = data{contador}(2); %Dividir segun ADC y calibracion
        cont2 = cont2 + 1;
    end
    contador = contador + 1;
    plot(data1)
    drawnow
end
fclose(puerto);
delete(puerto);
%Crear vector de tiempo segun periodo de muestreo
%