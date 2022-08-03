from time import sleep
from random import randint
from threading import Thread, Lock, Condition, current_thread

def produtor():
  global buffer
  for i in range(10):
    sleep(randint(0,2))           # fica um tempo produzindo...
    item = 'item ' + str(i)
    with lock:
      if len(buffer) == tam_buffer:
        print('>>> Buffer cheio. Produtor %s ira aguardar.' % current_thread().getName())
        lugar_no_buffer.wait()    # aguarda que haja lugar no buffer
      buffer.append(item)
      print('%s produziu %s (ha %i itens no buffer)' % (current_thread().getName(),item,len(buffer)))
      item_no_buffer.notify()

def consumidor():
  global buffer
  for i in range(10):
    with lock:
      if len(buffer) == 0:
        print(">>> Buffer vazio. Consumidor %s ira aguardar." % current_thread().getName())
        item_no_buffer.wait()   # aguarda que haja um item para consumir 
      item = buffer.pop(0)
      print('%s consumiu %s (ha %i itens no buffer)' % (current_thread().getName(),item,len(buffer)))
      lugar_no_buffer.notify()
    sleep(randint(0,2))         # fica um tempo consumindo...

buffer = []
tam_buffer = 5
lock = Lock()
lugar_no_buffer = Condition(lock)
item_no_buffer = Condition(lock)
produtor1 = Thread(target=produtor, name="p1")
produtor2 = Thread(target=produtor, name="p2") 
consumidor1 = Thread(target=consumidor, name="c1")
consumidor2 = Thread(target=consumidor, name="c2")
produtor1.start()
produtor2.start()
consumidor1.start()
consumidor2.start()
produtor1.join()
produtor2.join()
consumidor1.join()
consumidor2.join()