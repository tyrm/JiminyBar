#!/usr/bin/node
// Queries Puppet DB for the last 15 Runs and exports their status as a
// string of characters:
//  * U - Nochange
//  * C - Changed
//  * F - Failed
//  * N - Noop (notimplimented)
//  * 0 - Null

var limit = 15; // Number of runs to pull

var http = require("http");

var options = {
  host: 'localhost',
  port: '8080',
  path: '/v4/reports?limit=' + limit + '&order-by=' + encodeURIComponent('[{"field": "end-time", "order": "desc"}]')
};

var server = http.createServer(function(request, response) {
  console.log(request.url);
  switch(request.url) {
    case '/jiminy':
      callback = function(pup_response) {
        var str = '';

        //another chunk of data has been recieved, so append it to `str`
        pup_response.on('data', function(chunk) {
          str += chunk;
        });

        pup_response.on('error', function(e) {
          response.writeHead(200, {"Content-Type": "text/plain"});
          for (var i = 0; i < limit; i++) {
            response.write('0');
          };
          response.end();
          console.log('[PUP]' + e);
        });

        pup_response.on('end', function() {
          response.writeHead(200, {"Content-Type": "text/plain"});
          var reports = JSON.parse(str);

          var theString = "";

          for (var i = 0, len = reports.length; i < len; i++) {
            switch(reports[i].status) {
              case 'unchanged':
                theString = theString + 'U';
                break;
              case 'changed':
                theString = theString + 'C';
                break;
              case 'failed':
                theString = theString + 'F';
                break;
              default:
                theString = theStriung + '0';
            }
          };
          response.write(theString);
          response.end();
        });
      }

      var req = http.request(options, callback);

      req.on('error', function(e) {
        response.writeHead(200, {"Content-Type": "text/plain"});
        for (var i = 0; i < limit; i++) {
          response.write('0');
        };
        response.end();
        console.log('[REQ]' + e);
      });

      req.end();
      break;
    default:
      response.writeHead(404, {"Content-Type": "text/plain"});
      response.write('nope');
      response.end();
      break;
    }
});
 
server.listen(1492);
console.log("Server is listening");
