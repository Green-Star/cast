var socket = io();

socket.on('data updated', (data) => clientUpdateData(data));
