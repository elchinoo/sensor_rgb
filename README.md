# Sensor RGB
Sensor de Cor Baseado em Sensor RGB e Arduino para Automação de Esteira Separadora de Tampinhas.

## Descrição:
 Este projeto implementa um sensor de cor utilizando um módulo RGB integrado a um Arduino, com o objetivo de automatizar uma 
 esteira separadora de tampinhas. Desenvolvido no contexto do curso de Mecatrônica Industrial no Unisenai/SC, 
 este sistema será integrado a CLPs para ampliar sua aplicação em ambientes industriais. 

 O sistema é controlado por uma máquina de estados finitos, que gerencia os seguintes estados operacionais:
 - **INICIALIZACAO**: Configuração e validação inicial do sensor.
 - **LEITURA**: Captura e análise da cor das tampinhas.
 - **CONFIGURACAO VM**: Ajuste para detecção da cor vermelha.
 - **CONFIGURACAO VD**: Ajuste para detecção da cor verde.
 - **CONFIGURACAO AZ**: Ajuste para detecção da cor azul.
 - **RESET**: Retorno ao estado inicial ou reinicialização do sistema.

 ## Características principais:
 - Integração entre sensores RGB e controladores CLP para aplicações industriais.
 - Implementação modular baseada em máquina de estados finitos para maior controle e flexibilidade.
 - Capacidade de configuração dinâmica das cores a serem detectadas.
 - Projeto educativo com foco na automação e eficiência de processos industriais.


 Este código é fornecido como material didático e está sujeito a modificações para melhorias ou personalizações de acordo com as necessidades do projeto.
 
 Projeto sob a licensa GPL
