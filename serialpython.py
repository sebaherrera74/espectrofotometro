#!/usr/bin/python
#!/usr/bin/python3

# Importamos la libreria de PySerial
import time
import serial
import struct


PuertoSerie = serial.Serial('/dev/ttyUSB1', 115200)

from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg
from pyqtgraph.Point import Point

app = QtGui.QApplication([])
win = pg.GraphicsWindow(title="Grafica Recepcion de datos seriales educiaa")
p = win.addPlot(title="Grafica tiempo real")
curva = p.plot(pen='y')
p.setRange(yRange=[0, 1025])
p.setLabel('left', "Absorbancia")
p.setLabel('bottom', "Nanometros")



dataX = [] # lista para guardar los datos
dataY = []
lastY = 0

def Update():
    global curva, dataX, dataY,lastY
    # Leemos la nueva línea
    line = PuertoSerie.readline()
    nuevoDato = float(line.decode('utf-8'))
    print(str(nuevoDato))
    #Agregamos los datos al array
    
    dataX.append(nuevoDato)
    dataY.append(lastY)
    lastY = lastY + 1

    # Limitamos a mostrar solo 300 muestras
    if len(dataX) > 300:
      dataX = dataX[:-1]
      dataY = dataY[:-1]

    #Actualizamos los datos y refrescamos la gráfica.
    curva.setData(dataY, dataX)
    QtGui.QApplication.processEvents()

    
while True: 
  Update() #Actualizamos todo lo rápido que podamos.

"""## Start Qt event loop unless running in interactive mode or using pyside.
if __name__ == '__main__':
  import sys
  if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
    QtGui.QApplication.instance().exec_()
#pg.QtGui.QApplication.exec_()"""






