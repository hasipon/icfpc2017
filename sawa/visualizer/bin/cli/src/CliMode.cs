// Generated by Haxe 3.4.2 (git build master @ 890f8c7)

#pragma warning disable 109, 114, 219, 429, 168, 162
public class CliMode : global::haxe.lang.ParamEnum {
	
	public CliMode(int index, object[] @params) : base(index, @params) {
	}
	
	
	public static readonly string[] __hx_constructs = new string[]{"Off", "On"};
	
	public static readonly global::CliMode Off = new global::CliMode(0, null);
	
	public static global::CliMode On(string host, int port) {
		unchecked {
			return new global::CliMode(1, new object[]{host, port});
		}
	}
	
	
	public override string getTag() {
		return global::CliMode.__hx_constructs[this.index];
	}
	
	
}


