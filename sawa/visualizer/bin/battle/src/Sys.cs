// Generated by Haxe 3.4.2 (git build master @ 890f8c7)

#pragma warning disable 109, 114, 219, 429, 168, 162
public class Sys : global::haxe.lang.HxObject {
	
	public Sys(global::haxe.lang.EmptyObject empty) {
	}
	
	
	public Sys() {
		global::Sys.__hx_ctor__Sys(this);
	}
	
	
	public static void __hx_ctor__Sys(global::Sys __hx_this) {
	}
	
	
	public static int getChar(bool echo) {
		return ((int) (global::System.Console.ReadKey(((bool) ( ! (echo) ) )).KeyChar) );
	}
	
	
}


