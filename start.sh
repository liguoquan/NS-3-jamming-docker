sudo docker run -i -t \
  -v "${PWD}/scratch:/root/ns-allinone-3.11/ns-3.11/scratch" \
  -w /root/ns-allinone-3.11/ns-3.11 \
  ns-3:jamming \
  /bin/bash

USER_NAME=${SUDO_USER:=$USER}
USER_ID=$(id -u "${USER_NAME}")
GROUP_ID=$(id -g "${USER_NAME}")
sudo chown -R "${USER_ID}:${GROUP_ID}" scratch