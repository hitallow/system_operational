# Sistemas operacionais
Repositório voltado para a implementação de códigos avançados em C, com base no estudo de sistemas operacionais
Assuntos abordados:
<ul>
<hr>
<li>

  <div>
  <h4>Multi-processamento</h4>
  <p> Nesta parte é estudado um pouco do multiprocessamento que ocorre nas máquinas do nosso dia a dia, e como podemos fazer 
  proveito disto.</p>
  <p>
  O multiprocessamento tem suas vantagens e desvantagens,que podem afetar no resultado final, as nossas thread,
  podem dividir a mesma variável ou variáveis, ou um trecho de código pode ser semelhante para ambas threads. 
  <br/>
  Veja, algumas notações:
    <ul>
    <li>
      Condição de corrida: <p>Quando dois ou mais processos precisam acessar um recurso em comum.</p>
    </li>
    <li>
      Seção crítica : <p>Região de código acessada por mais de um processo. </p>
    </li>
    <li>
      Exclusão mútua: <p> Quando garantimos que um único processo, seja ele leve ou pesado, 
      possa acessar a seção crítica. Ou seja, quando a entrada de um processo na seção crítica, bloqueia a entrada de
      quaisquer outro processo.</p>
    </li>
  </ul>
  </p>

  <h5>Sabendo destes problemas, como podemos solucionar eventuais erros?</h5>
   <p>Um dos métodos mais utilizados e aqui apresentado, são os semáforos. Utilizamos a biblioteca 
   <code>dijkstra.h</code>aqui incluida. </p>
   <p>
    Os semáforos, garantem a exclusão mútua, evitando que outros processos acessem uma seção crítica. <br/>
    Vejamos a documentação de cada função:
    <ul>
      <li>
        <code>sem_create_d(key_t key, int initval)</code> :cria um semáforo, o primeiro paramêtro 
        <code>key</code> representa o uma chave única que identifica o semáforo para S.O., o segundo paramêtro
        indica qual o valor inicial do meu semáforo, logo, se quisermos garantir exclusão mútua, inserimos o valor 1, para o
        <code>initval</code>.
        <p><strong>Eventuais erros</strong>: Lembre-se, o key deve ser um valor único para cada semáforo, caso existam 
        dois semáforos com a mesma chave, seu código pode não funcionar da maneira esperada.
        </p>
      </li>
      <li>
        <code>wait_d(int semid)</code> : decrementa o valor do semáforo e 
        verifica o valor daquele semáforo. Logo, um semáforo que começa com o 
        <code>initval = 1</code> a função decrementa o valor do semafóro, e agora aquele semáforo agora tem o valor 0,
        Mas além de decrementar o valor ele verifica o valor do semafóro, caso o valor dele, seja menor ou igual a 0, 
        aquele processo entra em estado de bloqueado. Desta forma podemos garantir exclusão mútua.
      </li>
      <li>
        <code>signal_d(int semid)</code>: incrementa o valor do semáforo, e libera a seção crítica que estava 
        sendo utilizada. Logo, quando o processo termina de executar a seção crítica ele incrementa o valor do 
        semáforo. 
      </li>
      <li>
        <code>sem_delete(int semid)</code>: exclui um semáforo do SO.
      </li>
  </div>
  <hr>
</li>
</ul>
