package;
import js.Browser;
import js.Node;

class ServerMain 
{
    public static function main() 
    {
        
        startHttpServer();
    }
    
    private static function startHttpServer():Void
    {
        var options = { root: "site" }
        var server = Node.require('http-server').createServer(options);
        var host = "127.0.0.1";
        var port = 8080;
        
        server.listen(
            port,
            host,
            function ()
            {
                Node.console.log(
                    [
                        'Starting up http-server, serving ',
                        server.root,
                        '\nAvailable on:',
                        host + ":" + port,
                    ].join('')
                );
                Node.console.log('Hit CTRL-C to stop the server');
            }
        );
    }
}
