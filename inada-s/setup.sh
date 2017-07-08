# install glog
if [ ! -e /usr/local/lib/libglog.a ]; then
  git clone git@github.com:google/glog.git
  cd glog
  ./autogen.sh && ./configure && make && make install

  cd ..
  rm -r glog
fi

