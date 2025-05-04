import concurrent.futures
import time

start = time.perf_counter()

def do_something(seconds):
    print(f'Sleeping {seconds} second(s)....')
    time.sleep(seconds)
    print('Done Sleeping....')
    return f"It is finished in {seconds}"

threads = []

# old method
with concurrent.futures.ThreadPoolExecutor() as executor:
    """
    f1 = executor.submit(do_something, 1) #schedules a function and its arguments to be executed and returns a future object
    f2 = executor.submit(do_something, 1)
    """
    '''
    #submit does things concurrently but not in the order it was executed
    secs = [5,4,3,2,1]
    results = [executor.submit(do_something, sec) for sec in secs] #list comprehension
    for f in concurrent.futures.as_completed(results):
        print(f.result())
    '''

    #map also does things concurrently but it takes into consideration the order of the processes

    secs = [5,4,3,2,1]
    results = executor.map(do_something, secs) #takes into consideration arrays
    for result in results:
        print(result)
#new method
"""   
# a script that should be 10 seconds is now 1 second
for _ in range (10): # underscore is a throwaway variable
    t = threading.Thread(target=do_something, args = [1.5]) #argument of a list of one element 
    t.start()
    threads.append(t)

for thread in threads:
    thread.join()

"""

finish = time.perf_counter()
print(f'Finished in {round(finish-start,2)} second(s)')

