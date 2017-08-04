package;

enum CliMode 
{
    Off;
    On(host:String, port:Int);
}