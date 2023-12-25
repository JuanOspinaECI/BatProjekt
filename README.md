This project is dedicated to present a functional Ion-lithium battery cell balancing prototype that enhances the self-balancing batteries behavior introducing an innovative and efficient approach. 
Since parallel cell configurations are commonly used in commercial battery packs, the research primary goal is to develop an introductory model that can be easily adapted for more complex battery setups in future applications. 
Additionally, it will be used as a test circuit for analyzing battery charge and discharge behaviors.


Some initial test of the natural balancing conditions of the parallel batteries balancing (we had 10 different batteries for tests):

Batteries #1 and #7 voltage when balancing in parallel:

![alt text](https://github.com/JuanOspinaECI/BatProjekt/blob/main/IMG/Voltage_bal.png)

Batteries 1 and 7 voltage difference and current when balancing in parallel.

![alt text](https://github.com/JuanOspinaECI/BatProjekt/blob/main/IMG/Dif_V_n_Current.png)


This shows that there is some waste of energy while natural balancing, and the dominat battery needs to perform an additional cycle to discharge and then charge, if needed.

This is the graph that illustrates the natural behavior:

![alt text](https://github.com/JuanOspinaECI/BatProjekt/blob/main/IMG/batt_3_8_8Dominant..png)

A simple graph explaning the solution:

![alt text](https://github.com/JuanOspinaECI/BatProjekt/blob/main/IMG/BLOCKD%20(1).jpg)

A simplified flow diagram of the code:

![alt text](https://github.com/JuanOspinaECI/BatProjekt/blob/main/IMG/Flow_chart_simplified.png)

This shows the unbalanced charge with our solution:

![alt text](https://github.com/JuanOspinaECI/BatProjekt/blob/main/IMG/Charge_curve_bat3_5_zoom.png)

Here is the behavior with a load connected:

![alt text](https://github.com/JuanOspinaECI/BatProjekt/blob/main/IMG/discharge2_7.png)

When the batteries are balanced, the circuit is capable of charging both batteries with just one charger and maintaining the balance:

![alt text](https://github.com/JuanOspinaECI/BatProjekt/blob/main/IMG/charge2_7_parallel.png)



---------------------------------------------------------------------------------------------------


The data was acquired from the USART and read from a module to the USB port of the PC. The Matlab code is designed to take the data and store it in vectors. Subsequently, the data underwent additional treatment and processing to generate the graphs.

----------------------------------------------------------------------------------------------------

The following photos show the esquematic desing:





