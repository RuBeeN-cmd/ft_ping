FROM    	debian:bullseye

RUN			apt update && apt install -y inetutils-ping \
				make \
				gcc \
				zsh \
				curl \
				git

RUN			sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

WORKDIR		/ft_ping

RUN			ln -s /ft_ping/ft_ping /bin/ft_ping