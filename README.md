# <center>Global Solution 2 - [nome do jogo] </center>

## <center>O PROBLEMA</center>
&nbsp;&nbsp;&nbsp;&nbsp;O problema da saúde mundial, como destacada no ODS 3 (Objetivo de Desenvolvimento Sustentável) da ONU, se traduz em desafios complexos que afetam comunidades em todo mundo. Apesar dos avanços e melhorias, milhões de pessoas continuam sem acesso a serviços de saúde essenciais, enfrentando doenças evitáveis ​​e falta de cuidados básicos. Além disso, milhões de pessoas sofrem de doenças mentais/psicológicas como depressão ou ansiedade, que podem ser evitados com acompanhamento de um profissional ou até mesmo levando uma vida mais saudável, pois a saúde não só afeta o físico, mas também a mente e as relações sociais. Por isso é essencial que tenhamos uma vida em paz e harmonia com a saúde para podermos aproveitar a vida ao máximo.

## <center>A SOLUÇÃO</center>
### Projeto consiste em um meio de incentivar pessoas a ter habitos mais saúdaveis através de um jogo.
&nbsp;&nbsp;&nbsp;&nbsp;Muitas pessoas levam uma vida não saudável por conta da falta de informação facil e pela **falta de atenção** da pessoa. Apesar da internet disponibilizar inumeras maneiras de ter uma vida saudável, para uma melhoria significativa, é necessario o individuo ter o interesse em ter uma vida melhor, o que não acontece quando a pessoa precisa ver um vídeo longo ou ler sobre tal assunto. Devido a isso, criamos um **jogo** onde as intruções serão dadas em forma de missões personalizadas dependendo do perfil de cada pessoa, e tais missões darão recompensas para que o usuário sinta que o que ele faz trará algum progresso tanto para a vida dele, como digitalmente, onde é mais visivel. O jogador poderá ver as suas *skills* e seu progresso será de acordo com o progresso da vida real, através de **sensores** onde pegarão dados reais da pessoa para que a missão dentro do jogo seja completada.<br>
&nbsp;&nbsp;&nbsp;&nbsp;Nesta etapa inicial do projeto, usaremos o ESP32 no Wokwi para simular valores de BPM, passos e calorias queimadas com sensores LDR, DHT22 e Ultrasonico, respectivamente. Apesar dos sensores não serem usados para tais medimentos, pela falta de sensores, usaremos esses sensores com valores semelhantes a valores que serão captados de BPM, passos e calorias. Usaremos um broker MQTT para que o ESP32 possa se comunicar via wi-fi e transmitir os dados para um tópico especifico, node-red como intermediario de comunicação e um CSP (Cloud Service Provider) para melhor visualização dos dados.

## <center>INSTRUÇÕES</center>
* Abra o Wokwi em https://wokwi.com/ e crie um novo projeto ESP32.
* Importe as bibliotecas do arduino, dht e pubsubclient que está em Wokwi/libraries.txt ou na aba Library Manager.
* Monte um circuito com 3 LEDS verdes, 3 resistores 220, 1 LDR, 1 DHT22 e 1 Ultrasonico ou importe o diagrama em Wokwi/diagram.json
* Importe o código em Wokwi/sketch.ino. Caso necessário mude o ID_MQTT no código ou os tópicos em *TOPIC_PUBLISH.
* Abra o MQTT Websocket em https://www.hivemq.com/demos/websocket-client/ e conecte-se. Se increva-se no tópico fiap/LifeGame/# para visualizar todos os dados ou subistitua o '#' por algum tópico especifico (ex.: fiap/LifeGame/bpm).
* Caso não tenha, instale o Node-RED e abra-o localmente. Em seguida importe o flow em FlowNode_Red/flows.json
* Faça as alterações necessárias no Node-RED caso tenha mudado o tópico, e **altere** o node mqtt-out que passa as informações para o Tago com seu ID e Token do device desejado. 
* Execute o programa no Wokwi e espere a simulação rodar.
* Veja se está tudo correto com as informações de debug no próprio Wokwi, no MQTT, no debug do Node-RED e no TagoIO.
* Crie um Dashboard com o widget de escolha pessoal e selecione o dado que quer visualizar.

### <center>Link do projeto Wokwi: https://wokwi.com/projects/381610265969598465</center>