Comm
----

Comm is a simple inter-process communication protocol for communicating over linux sockets.

There are two classes comm::master and comm:slave that are simple implementations of a
bidirectional multiplexed socket server and client. They work over a request based system
with comm::master having functions to request a "write" and "read" to a device and comm::slave
having matching register functions that can register a function pointer to a "write" or "read" request.

Here are the message implementations. Each message is determined by the first letter.

"w[d][val]"   - write the specified device to given value
"r[d]"        - request a read of the specified device
"q[d]"        - request memory update of specified device
"l[d]"        - get and lock a memory block
"u[d]"        - unlock a memory block
"p"           - ping the item
"!"           - 'panic', should reset everything
"a[d]"        - successful action response
"d[d][val]"   - read data from the device
"c"           - successful ping response
"x"           - failure response
