// Generated by Haxe 3.4.2 (git build master @ 890f8c7)

#pragma warning disable 109, 114, 219, 429, 168, 162
namespace haxe.format {
	public class JsonParser : global::haxe.lang.HxObject {
		
		public JsonParser(global::haxe.lang.EmptyObject empty) {
		}
		
		
		public JsonParser(string str) {
			global::haxe.format.JsonParser.__hx_ctor_haxe_format_JsonParser(this, str);
		}
		
		
		public static void __hx_ctor_haxe_format_JsonParser(global::haxe.format.JsonParser __hx_this, string str) {
			__hx_this.str = str;
			__hx_this.pos = 0;
		}
		
		
		public string str;
		
		public int pos;
		
		public virtual object parseRec() {
			unchecked {
				while (true) {
					string s = this.str;
					int index = this.pos++;
					int c = ( (((bool) (( ((uint) (index) ) < s.Length )) )) ? (((int) (s[index]) )) : (-1) );
					switch (c) {
						case 9:
						case 10:
						case 13:
						case 32:
						{
							break;
						}
						
						
						case 34:
						{
							return this.parseString();
						}
						
						
						case 45:
						case 48:
						case 49:
						case 50:
						case 51:
						case 52:
						case 53:
						case 54:
						case 55:
						case 56:
						case 57:
						{
							int c1 = c;
							int start = ( this.pos - 1 );
							bool minus = ( c1 == 45 );
							bool digit =  ! (minus) ;
							bool zero = ( c1 == 48 );
							bool point = false;
							bool e = false;
							bool pm = false;
							bool end = false;
							while (true) {
								string s1 = this.str;
								int index1 = this.pos++;
								if (((bool) (( ((uint) (index1) ) < s1.Length )) )) {
									c1 = ((int) (s1[index1]) );
								}
								else {
									c1 = -1;
								}
								
								switch (c1) {
									case 43:
									case 45:
									{
										if ((  ! (e)  || pm )) {
											this.invalidNumber(start);
										}
										
										digit = false;
										pm = true;
										break;
									}
									
									
									case 46:
									{
										if (( minus || point )) {
											this.invalidNumber(start);
										}
										
										digit = false;
										point = true;
										break;
									}
									
									
									case 48:
									{
										if (( zero &&  ! (point)  )) {
											this.invalidNumber(start);
										}
										
										if (minus) {
											minus = false;
											zero = true;
										}
										
										digit = true;
										break;
									}
									
									
									case 49:
									case 50:
									case 51:
									case 52:
									case 53:
									case 54:
									case 55:
									case 56:
									case 57:
									{
										if (( zero &&  ! (point)  )) {
											this.invalidNumber(start);
										}
										
										if (minus) {
											minus = false;
										}
										
										digit = true;
										zero = false;
										break;
									}
									
									
									case 69:
									case 101:
									{
										if (( ( minus || zero ) || e )) {
											this.invalidNumber(start);
										}
										
										digit = false;
										e = true;
										break;
									}
									
									
									default:
									{
										if ( ! (digit) ) {
											this.invalidNumber(start);
										}
										
										this.pos--;
										end = true;
										break;
									}
									
								}
								
								if (end) {
									break;
								}
								
							}
							
							double f = global::Std.parseFloat(global::haxe.lang.StringExt.substr(this.str, start, new global::haxe.lang.Null<int>(( this.pos - start ), true)));
							int i = ((int) (f) );
							if (( i == f )) {
								return i;
							}
							else {
								return f;
							}
							
						}
						
						
						case 91:
						{
							global::Array arr = new global::Array<object>(new object[]{});
							global::haxe.lang.Null<bool> comma = default(global::haxe.lang.Null<bool>);
							while (true) {
								string s2 = this.str;
								int index2 = this.pos++;
								int c2 = ( (((bool) (( ((uint) (index2) ) < s2.Length )) )) ? (((int) (s2[index2]) )) : (-1) );
								switch (c2) {
									case 9:
									case 10:
									case 13:
									case 32:
									{
										break;
									}
									
									
									case 44:
									{
										if (((comma)).@value) {
											comma = new global::haxe.lang.Null<bool>(false, true);
										}
										else {
											this.invalidChar();
										}
										
										break;
									}
									
									
									case 93:
									{
										if (global::haxe.lang.Runtime.eq((comma).toDynamic(), false)) {
											this.invalidChar();
										}
										
										return arr;
									}
									
									
									default:
									{
										if (((comma)).@value) {
											this.invalidChar();
										}
										
										this.pos--;
										global::haxe.lang.Runtime.callField(arr, "push", 1247875546, new global::Array<object>(new object[]{this.parseRec()}));
										comma = new global::haxe.lang.Null<bool>(true, true);
										break;
									}
									
								}
								
							}
							
							break;
						}
						
						
						case 102:
						{
							int save = this.pos;
							bool tmp = default(bool);
							bool tmp1 = default(bool);
							bool tmp2 = default(bool);
							string s3 = this.str;
							int index3 = this.pos++;
							if (( (( (((bool) (( ((uint) (index3) ) < s3.Length )) )) ? (((int) (s3[index3]) )) : (-1) )) == 97 )) {
								string s4 = this.str;
								int index4 = this.pos++;
								tmp2 = ( (( (((bool) (( ((uint) (index4) ) < s4.Length )) )) ? (((int) (s4[index4]) )) : (-1) )) != 108 );
							}
							else {
								tmp2 = true;
							}
							
							if ( ! (tmp2) ) {
								string s5 = this.str;
								int index5 = this.pos++;
								tmp1 = ( (( (((bool) (( ((uint) (index5) ) < s5.Length )) )) ? (((int) (s5[index5]) )) : (-1) )) != 115 );
							}
							else {
								tmp1 = true;
							}
							
							if ( ! (tmp1) ) {
								string s6 = this.str;
								int index6 = this.pos++;
								tmp = ( (( (((bool) (( ((uint) (index6) ) < s6.Length )) )) ? (((int) (s6[index6]) )) : (-1) )) != 101 );
							}
							else {
								tmp = true;
							}
							
							if (tmp) {
								this.pos = save;
								this.invalidChar();
							}
							
							return false;
						}
						
						
						case 110:
						{
							int save1 = this.pos;
							bool tmp3 = default(bool);
							bool tmp4 = default(bool);
							string s7 = this.str;
							int index7 = this.pos++;
							if (( (( (((bool) (( ((uint) (index7) ) < s7.Length )) )) ? (((int) (s7[index7]) )) : (-1) )) == 117 )) {
								string s8 = this.str;
								int index8 = this.pos++;
								tmp4 = ( (( (((bool) (( ((uint) (index8) ) < s8.Length )) )) ? (((int) (s8[index8]) )) : (-1) )) != 108 );
							}
							else {
								tmp4 = true;
							}
							
							if ( ! (tmp4) ) {
								string s9 = this.str;
								int index9 = this.pos++;
								tmp3 = ( (( (((bool) (( ((uint) (index9) ) < s9.Length )) )) ? (((int) (s9[index9]) )) : (-1) )) != 108 );
							}
							else {
								tmp3 = true;
							}
							
							if (tmp3) {
								this.pos = save1;
								this.invalidChar();
							}
							
							return null;
						}
						
						
						case 116:
						{
							int save2 = this.pos;
							bool tmp5 = default(bool);
							bool tmp6 = default(bool);
							string s10 = this.str;
							int index10 = this.pos++;
							if (( (( (((bool) (( ((uint) (index10) ) < s10.Length )) )) ? (((int) (s10[index10]) )) : (-1) )) == 114 )) {
								string s11 = this.str;
								int index11 = this.pos++;
								tmp6 = ( (( (((bool) (( ((uint) (index11) ) < s11.Length )) )) ? (((int) (s11[index11]) )) : (-1) )) != 117 );
							}
							else {
								tmp6 = true;
							}
							
							if ( ! (tmp6) ) {
								string s12 = this.str;
								int index12 = this.pos++;
								tmp5 = ( (( (((bool) (( ((uint) (index12) ) < s12.Length )) )) ? (((int) (s12[index12]) )) : (-1) )) != 101 );
							}
							else {
								tmp5 = true;
							}
							
							if (tmp5) {
								this.pos = save2;
								this.invalidChar();
							}
							
							return true;
						}
						
						
						case 123:
						{
							object obj = new global::haxe.lang.DynamicObject(new int[]{}, new object[]{}, new int[]{}, new double[]{});
							string field = null;
							global::haxe.lang.Null<bool> comma1 = default(global::haxe.lang.Null<bool>);
							while (true) {
								string s13 = this.str;
								int index13 = this.pos++;
								int c3 = ( (((bool) (( ((uint) (index13) ) < s13.Length )) )) ? (((int) (s13[index13]) )) : (-1) );
								switch (c3) {
									case 9:
									case 10:
									case 13:
									case 32:
									{
										break;
									}
									
									
									case 34:
									{
										if (((comma1)).@value) {
											this.invalidChar();
										}
										
										field = this.parseString();
										break;
									}
									
									
									case 44:
									{
										if (((comma1)).@value) {
											comma1 = new global::haxe.lang.Null<bool>(false, true);
										}
										else {
											this.invalidChar();
										}
										
										break;
									}
									
									
									case 58:
									{
										if (string.Equals(field, null)) {
											this.invalidChar();
										}
										
										global::Reflect.setField(obj, field, this.parseRec());
										field = null;
										comma1 = new global::haxe.lang.Null<bool>(true, true);
										break;
									}
									
									
									case 125:
									{
										if ((  ! (string.Equals(field, null))  || global::haxe.lang.Runtime.eq((comma1).toDynamic(), false) )) {
											this.invalidChar();
										}
										
										return obj;
									}
									
									
									default:
									{
										this.invalidChar();
										break;
									}
									
								}
								
							}
							
							break;
						}
						
						
						default:
						{
							this.invalidChar();
							break;
						}
						
					}
					
				}
				
				return null;
			}
		}
		
		
		public virtual string parseString() {
			unchecked {
				int start = this.pos;
				global::StringBuf buf = null;
				while (true) {
					string s = this.str;
					int index = this.pos++;
					int c = ( (((bool) (( ((uint) (index) ) < s.Length )) )) ? (((int) (s[index]) )) : (-1) );
					if (( c == 34 )) {
						break;
					}
					
					if (( c == 92 )) {
						if (( buf == null )) {
							buf = new global::StringBuf();
						}
						
						{
							string s1 = this.str;
							global::haxe.lang.Null<int> len = new global::haxe.lang.Null<int>(( ( this.pos - start ) - 1 ), true);
							buf.b.Append(((string) (s1) ), ((int) (start) ), ((int) (( ( ! (len.hasValue) ) ? (( s1.Length - start )) : ((len).@value) )) ));
						}
						
						string s2 = this.str;
						int index1 = this.pos++;
						if (((bool) (( ((uint) (index1) ) < s2.Length )) )) {
							c = ((int) (s2[index1]) );
						}
						else {
							c = -1;
						}
						
						switch (c) {
							case 34:
							case 47:
							case 92:
							{
								buf.b.Append(((char) (c) ));
								break;
							}
							
							
							case 98:
							{
								buf.b.Append(((char) (8) ));
								break;
							}
							
							
							case 102:
							{
								buf.b.Append(((char) (12) ));
								break;
							}
							
							
							case 110:
							{
								buf.b.Append(((char) (10) ));
								break;
							}
							
							
							case 114:
							{
								buf.b.Append(((char) (13) ));
								break;
							}
							
							
							case 116:
							{
								buf.b.Append(((char) (9) ));
								break;
							}
							
							
							case 117:
							{
								global::haxe.lang.Null<int> uc = global::Std.parseInt(global::haxe.lang.Runtime.concat("0x", global::haxe.lang.StringExt.substr(this.str, this.pos, new global::haxe.lang.Null<int>(4, true))));
								this.pos += 4;
								buf.b.Append(((char) ((uc).@value) ));
								break;
							}
							
							
							default:
							{
								throw global::haxe.lang.HaxeException.wrap(global::haxe.lang.Runtime.concat(global::haxe.lang.Runtime.concat(global::haxe.lang.Runtime.concat("Invalid escape sequence \\", new string(((char) (c) ), 1)), " at position "), global::haxe.lang.Runtime.toString((( this.pos - 1 )))));
							}
							
						}
						
						start = this.pos;
					}
					else if (( c == -1 )) {
						throw global::haxe.lang.HaxeException.wrap("Unclosed string");
					}
					
				}
				
				if (( buf == null )) {
					return global::haxe.lang.StringExt.substr(this.str, start, new global::haxe.lang.Null<int>(( ( this.pos - start ) - 1 ), true));
				}
				else {
					{
						string s3 = this.str;
						global::haxe.lang.Null<int> len1 = new global::haxe.lang.Null<int>(( ( this.pos - start ) - 1 ), true);
						buf.b.Append(((string) (s3) ), ((int) (start) ), ((int) (( ( ! (len1.hasValue) ) ? (( s3.Length - start )) : ((len1).@value) )) ));
					}
					
					return buf.b.ToString();
				}
				
			}
		}
		
		
		public virtual void invalidChar() {
			unchecked {
				this.pos--;
				string s = this.str;
				int index = this.pos;
				throw global::haxe.lang.HaxeException.wrap(global::haxe.lang.Runtime.concat(global::haxe.lang.Runtime.concat(global::haxe.lang.Runtime.concat("Invalid char ", global::haxe.lang.Runtime.toString((( (((bool) (( ((uint) (index) ) < s.Length )) )) ? (((int) (s[index]) )) : (-1) )))), " at position "), global::haxe.lang.Runtime.toString(this.pos)));
			}
		}
		
		
		public virtual void invalidNumber(int start) {
			throw global::haxe.lang.HaxeException.wrap(global::haxe.lang.Runtime.concat(global::haxe.lang.Runtime.concat(global::haxe.lang.Runtime.concat("Invalid number at position ", global::haxe.lang.Runtime.toString(start)), ": "), global::haxe.lang.StringExt.substr(this.str, start, new global::haxe.lang.Null<int>(( this.pos - start ), true))));
		}
		
		
		public override double __hx_setField_f(string field, int hash, double @value, bool handleProperties) {
			unchecked {
				switch (hash) {
					case 5594516:
					{
						this.pos = ((int) (@value) );
						return @value;
					}
					
					
					default:
					{
						return base.__hx_setField_f(field, hash, @value, handleProperties);
					}
					
				}
				
			}
		}
		
		
		public override object __hx_setField(string field, int hash, object @value, bool handleProperties) {
			unchecked {
				switch (hash) {
					case 5594516:
					{
						this.pos = ((int) (global::haxe.lang.Runtime.toInt(@value)) );
						return @value;
					}
					
					
					case 5744817:
					{
						this.str = global::haxe.lang.Runtime.toString(@value);
						return @value;
					}
					
					
					default:
					{
						return base.__hx_setField(field, hash, @value, handleProperties);
					}
					
				}
				
			}
		}
		
		
		public override object __hx_getField(string field, int hash, bool throwErrors, bool isCheck, bool handleProperties) {
			unchecked {
				switch (hash) {
					case 652994848:
					{
						return ((global::haxe.lang.Function) (new global::haxe.lang.Closure(this, "invalidNumber", 652994848)) );
					}
					
					
					case 701410669:
					{
						return ((global::haxe.lang.Function) (new global::haxe.lang.Closure(this, "invalidChar", 701410669)) );
					}
					
					
					case 208459108:
					{
						return ((global::haxe.lang.Function) (new global::haxe.lang.Closure(this, "parseString", 208459108)) );
					}
					
					
					case 1450762973:
					{
						return ((global::haxe.lang.Function) (new global::haxe.lang.Closure(this, "parseRec", 1450762973)) );
					}
					
					
					case 5594516:
					{
						return this.pos;
					}
					
					
					case 5744817:
					{
						return this.str;
					}
					
					
					default:
					{
						return base.__hx_getField(field, hash, throwErrors, isCheck, handleProperties);
					}
					
				}
				
			}
		}
		
		
		public override double __hx_getField_f(string field, int hash, bool throwErrors, bool handleProperties) {
			unchecked {
				switch (hash) {
					case 5594516:
					{
						return ((double) (this.pos) );
					}
					
					
					default:
					{
						return base.__hx_getField_f(field, hash, throwErrors, handleProperties);
					}
					
				}
				
			}
		}
		
		
		public override object __hx_invokeField(string field, int hash, global::Array dynargs) {
			unchecked {
				switch (hash) {
					case 652994848:
					{
						this.invalidNumber(((int) (global::haxe.lang.Runtime.toInt(dynargs[0])) ));
						break;
					}
					
					
					case 701410669:
					{
						this.invalidChar();
						break;
					}
					
					
					case 208459108:
					{
						return this.parseString();
					}
					
					
					case 1450762973:
					{
						return this.parseRec();
					}
					
					
					default:
					{
						return base.__hx_invokeField(field, hash, dynargs);
					}
					
				}
				
				return null;
			}
		}
		
		
		public override void __hx_getFields(global::Array<object> baseArr) {
			baseArr.push("pos");
			baseArr.push("str");
			base.__hx_getFields(baseArr);
		}
		
		
	}
}


