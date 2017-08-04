package;
import haxe.io.Bytes;
import haxe.macro.Context;
import sys.FileSystem;
import sys.io.File;

class MacroMain 
{
    public static function main():Void
    {
        var dir = "../../maps";
        for (file in FileSystem.readDirectory(dir))
        {
            if (StringTools.endsWith(file, ".json"))
            {
                Context.addResource(file, Bytes.ofString(File.getContent(dir + "/" + file)));
            }
        }
    }
}
