sudo docker run -i -t \
  -v "${PWD}/scratch:/root/ns-allinone-3.11/ns-3.11/scratch" \
  ns-3:jamming \
  /bin/bash