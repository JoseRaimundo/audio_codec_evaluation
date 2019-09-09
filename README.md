
### Cenário

![Base diagram](https://github.com/JoseRaimundo/audio_codec_evaluation/blob/master/img/base_diagram.png?raw=true)

### Proposta de Pesquisa

 - Avaliação de diversos protocolos de envio de pacotes: UDP, TCP, **RTP**, RTCP.
 - Avaliação de diverso codecs de audio ([Ver lista aqui!](https://www.ffmpeg.org/general.html#toc-Audio-Codecs)).
 
### Teoria

#### Real Time Protocol

RTP é um protocolo utilizado para o transporte de mídias contínuas de tempo real em uma conexão ponto a ponto, como áudio, vídeo ou dados de uma simulação. Pode ser usado em comuniçaões ponto a ponto ou broadcast, utilizando um endereço IP da faixa reservada para grupos. **Este protocolo não reserva recursos nem garante qualidade de serviço (QoS)**, porém ele é frequentemente utilizado em paralelo com o RTCP (_RTP Control Protocol_) permitindo que haja uma certa monitoração da comunicação. O uso deste protocolo será descrito na sessão seguinte deste documento. RTP e RTCP são utilizados paralelamente mas os pacotes de cada protocolo são transmitidos de forma independente. Na Figura abaixo, é apresentado a estrutura do cabeçalho RTP.

![CabecalhoRTP](https://github.com/JoseRaimundo/audio_codec_evaluation/blob/master/img/rtp_cabecalho.png?raw=true)

 **V – versão, (2 bits)**. Usado para especificar a versão do RTP.

0 : usado para especificar o primeiro protocolo utilizado na ferramenta de áudio “vat”.  
1: especifica a primeira versão do RTP utilizada como teste.  
2: identifica a versão do RTP especificada na RFC 1889.

**P – Preenchimento/padding, (1 bit)**. Sinaliza a adição de octetos de enchimento adicionais ao conteúdo da carga (payload) sem fazer parte da mesma. O último octeto do preenchimento contém a informação de quantos octetos foram inseridos. Este preenchimento adicional é normalmente utilizado para uso de algoritmos de criptografia de tamanho de blocos fixos ou para transmissão de pequenos conteúdos.

**X – Extensão/extension**, **(1 bit)**. Com esse bit marcado, é acrescentado uma extensão ao cabeçalho original.

**CC - Contador CSRC/CSRC count, (4 bits)**. Este campo contém o número de identificadores CSRC.

**M – marcador/marker, (1 bit)**. Usado para identificar as fronteiras de um quadro numa corrente de pacotes.

**PT – Tipo de carga/payload type, (7 bits**). Este campo identifica o formato da carga do pacote RTP como também a determinação de sua interpretação pela aplicação.

**Numeração seqüenciada/sequence number, (16 bits)** – A numeração seqüenciada põe em ordem os diversos pacotes de RTP. A cada novo pacote, a numeração é incrementada de uma unidade. Basicamente, esse ordenamento serve para o receptor detectar os pacotes perdidos e restaurar a seqüência de pacotes.  
**Selo de temporização\timestamp, (16 bits)**. Esse campo reflete o instante de amostragem do primeiro octeto no pacote RTP.

**SSRC, (32 bits)**. Esse campo identifica a fonte de sincronismo. Esta identificação foi escolhida aleatoriamente tencionando-se que duas fontes de sincronismo com a mesma sessão RTP não teriam o mesmo identificador SSRC.  

**CSRC, (itens de 0 a 15, 32 bits cada)**. A lista SCRC identifica a contribuição da fonte no conteúdo da carga (payload) de cada pacote. O número de identificadores é dado pelo campo CC. Se houver mais de 15 fontes contribuintes, somente 15 serão identificadas.

### Experimento

##### Simples

É possível realizar um experimento simples utilizando a ferramenta ffmpeg, como no exemplo:

Codificador:

> ffmpeg -re -acodec pcm_s16le -i test.wav -f rtp rtp://127.0.0.1:1234

Decoder (A identificação do codec no decoder é automática)

> ffplay -acodec pcm_s16le rtp://127.0.0.1:1234

##### Avançado

Para uma análise mais aprofundada e automatizada da manipulação dos pacotes, é possível utilizar a API FFmpeg, como é apresentado a seguir: 
(Neste exemplo, é criado um sinal a partir de um sino e codificado com codec mp2).


	static  void  audio_encode_example(const  char  *filename) {
		// Variáveis do codec e do stream
		AVCodec *codec;
		AVCodecContext *c=  NULL;
		int frame_size, i, j, out_size, outbuf_size;
		FILE *f;
		short  *samples;
		float t, tincr;
		uint8_t  *outbuf;
	
		// Seleciona o encoder
		codec =  avcodec_find_encoder(CODEC_ID_MP2);
		c=  avcodec_alloc_context();
		
		// Define os parâmetros do arquivo
		c->bit_rate  =  64000;
		c->sample_rate  =  44100;
		c->channels  =  2;		  

		// Abre o arquivo
		if (avcodec_open(c, codec) <  0) {
			fprintf(stderr, "could not open codec\n");
			exit(1);
		}
		
		// Cria um buffer para as amostras
		frame_size =  c->frame_size;
		samples =  malloc(frame_size *  2  *  c->channels);
		outbuf_size =  10000;
		outbuf =  malloc(outbuf_size);		  

		f =  fopen(filename, "wb");
		if (!f) {
			fprintf(stderr, "could not open %s\n", filename);
			exit(1);
		}

		// Codifica um som de exemplo, sino
		t =  0;
		tincr =  2  * M_PI *  440.0  /  c->sample_rate;
		for(i=0;i<200;i++) {
			for(j=0;j<frame_size;j++) {
				samples[2*j] = (int)(sin(t) *  10000);
				samples[2*j+1] =  samples[2*j];
				t += tincr;
			}
			// codifica o stream em outro (podendo ser arquivo ou stream de rede)
			out_size =  avcodec_encode_audio(c, outbuf, outbuf_size, samples);
			fwrite(outbuf, 1, out_size, f);
		}
		// Libera memória
		fclose(f);
		free(outbuf);
		free(samples);
		avcodec_close(c);
		av_free(c);
	}


Decoder:


	static  void  audio_decode_example(const  char  *outfilename, const  char  *filename){
		//Declaro as variáveis do decoder
		AVCodec *codec;
		AVCodecContext *c=  NULL;
		int out_size, len;
		// Buffer que vão receber o stream
		FILE *f, *outfile;
		uint8_t  *outbuf;
		uint8_t  inbuf[AUDIO_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
		AVPacket avpkt;
		  
		av_init_packet(&avpkt);
		
  
		// Define o decoder
		codec =  avcodec_find_decoder(CODEC_ID_MP2);
		if (!codec) {
			fprintf(stderr, "codec not found\n");
			exit(1);
		}
		c=  avcodec_alloc_context();
	 
		// Abre o arquivo do stream
		if (avcodec_open(c, codec) <  0) {
			fprintf(stderr, "could not open codec\n");
			exit(1);
		}
		
		// Reserva a memória para o stream de saída
		outbuf =  malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE);
		
		f =  fopen(filename, "rb");
		if (!f) {
			fprintf(stderr, "could not open %s\n", filename);
			exit(1);
		}
		
		outfile =  fopen(outfilename, "wb");
		if (!outfile) {
		av_free(c);
		exit(1);
		}
		
		// Carrega o buffer do stream na memoria (Contínuo)
		avpkt.data  = inbuf;
		avpkt.size  =  fread(inbuf, 1, AUDIO_INBUF_SIZE, f);
	 
		// Decodifica até o final do stream EOF
		while (avpkt.size  >  0) {
			out_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;
			len =  avcodec_decode_audio3(c, (short  *)outbuf, &out_size, &avpkt);
			if (len <  0) {
				fprintf(stderr, "Error while decoding\n");
				exit(1);
			}
			if (out_size >  0) {
				/* if a frame has been decoded, output it */
				fwrite(outbuf, 1, out_size, outfile);
			}
			avpkt.size  -= len;
			avpkt.data  += len;
			if (avpkt.size  < AUDIO_REFILL_THRESH) {
				/* Refill the input buffer, to avoid trying to decode
				* incomplete frames. Instead of this, one could also use
				* a parser, or use a proper container format through
				* libavformat. */
				memmove(inbuf, avpkt.data, avpkt.size);
				avpkt.data  = inbuf;
				len =  fread(avpkt.data  +  avpkt.size, 1,
				AUDIO_INBUF_SIZE -  avpkt.size, f);
				if (len >  0){
					avpkt.size  += len;
				}
			}
		}

		fclose(outfile);
		fclose(f);
		free(outbuf);
		avcodec_close(c);
		av_free(c);
	}








### Sites úteis

[https://trac.ffmpeg.org/wiki/audio%20types](https://trac.ffmpeg.org/wiki/audio%20types)  [https://www.wowza.com/docs/how-to-restream-using-ffmpeg-with-wowza-streaming-engine](https://www.wowza.com/docs/how-to-restream-using-ffmpeg-with-wowza-streaming-engine)


### Referência




- <https://www.gta.ufrj.br/grad/03_1/rtp/rtp31.htm>
- <https://www.gta.ufrj.br/grad/01_2/vidconf/rtp.html>

