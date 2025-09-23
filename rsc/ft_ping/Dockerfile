FROM    	debian:bullseye

RUN			echo "wireshark-common wireshark-common/install-setuid boolean true" | debconf-set-selections \
				&& apt install -y wireshark

RUN			apt update && DEBIAN_FRONTEND=noninteractive apt install -y inetutils-ping \
				make \
				gcc \
				zsh \
				curl \
				git \
				valgrind \
				wireshark
				
RUN			sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

WORKDIR		/ft_ping

RUN			ln -s /ft_ping/ft_ping /bin/ft_ping