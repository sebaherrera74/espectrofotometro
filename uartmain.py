import serial
import random
import time
import sys
import signal

from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg
from pyqtgraph.Point import Point


PuertoSerie = serial.Serial('/dev/ttyUSB1', 115200)
dataX = [] # lista para guardar los datos
dataY = []
lastY = 0
app = QtGui.QApplication([])

# Variables globales con valores iniciales ------------------------------------
def signal_handler(sig, frame):
    print('Se pulso Ctrl+C!, saliendo del programa!')
    try:
        ser.close()
    except:
        pass
    else:
        print('Se cerro el puerto OK.')
    sys.exit(0)


# comando help: Imprime la lista de comandos
def command_list():
   print( "Comandos disponibles -----------------------------------------------" )
   print( "   'h' (help) imprime esta lista de comandos." )
   print( "   'q' (quit) Salir del programa." )
   print( "   '1' Ensayo de Longitud de Onda Determinada." )
   print( "   '2' Ensayo tipo Barrido Longitud de Onda." )
   print( "--------------------------------------------------------------------\n" )
   return

def ELOD():
    print("Ensayo Longitud de Onda Determinada")
    cont=0
    while(1):
        fichero=open('informe_sensores.csv','a')  #puede ir afuera del while
        lectura=PuertoSerie.readline()
        print(str(lectura))
        nuevoDato = float(lectura.decode('utf-8'))
        print(str(nuevoDato))
        fichero.write(str(nuevoDato)+',')

        #cuento 2 veces para recibir los datos y recien le coloco un final de linea
        cont=cont+1
        if cont==2:
            fichero.write('\n')
            cont=0

    

    return

def EBLO():
    print("Ensayo Barrido Longitud de Onda ")
    win = pg.GraphicsWindow(title="Grafica Recepcion de datos seriales educiaa")
    p = win.addPlot(title="Grafica tiempo real")
    p.setRange(yRange=[0, 1025])
    p.setLabel('left', "Absorbancia")
    p.setLabel('bottom', "Nanometros")
    curva = p.plot(pen='y')
    
    aux=300
    while (aux>0):
        Update(curva) #Actualizamos todo lo rápido que podamos.
        aux=aux-1
        print(str(aux))

    return




# Inicializa y abre el puertos serie ------------------------------------------
def uart_main():
    command_list()           # Imprime la lista de comandos

  # Ciclo infinito hasta comando exit (q) ---------------------------------  
    while True:
        ensayos = {'1': ELOD,'2': EBLO,}

        command=""

        command = input(">> ")      # for Python 3
        if command == 'q':
            print("Puerto cerrado. Se cierra el programa.")
            #ser.close()
            exit()
        elif command == 'h':
            command_list()
        elif command in ensayos:
            ensayos[command]()
        else:
            print("Comando no conocido.")

def Update(curva):
    global dataX, dataY,lastY
    # Leemos la nueva línea
    
    line = PuertoSerie.readline()
    nuevoDato = float(line.decode('utf-8'))
    print(str(nuevoDato))
    #Agregamos los datos al array
    
    dataX.append(nuevoDato)
    dataY.append(lastY)
    lastY = lastY + 1

    # Limitamos a mostrar solo 300 muestras
    if len(dataX) > 200:
        dataX = dataX[:-1]
        dataY = dataY[:-1]

    #Actualizamos los datos y refrescamos la gráfica.
    curva.setData(dataY, dataX)
    QtGui.QApplication.processEvents()

         


signal.signal(signal.SIGINT, signal_handler)

def main():
    uart_main()





if __name__ == "__main__":
    main()





