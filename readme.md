# Mostrador Automotivo Digital - Uno Mille / Palio 🚗💨

Projeto de instrumentação digital personalizada desenvolvido para veículos da linha Fiat (Uno Mille Fire / Palio), utilizando um microcontrolador **ESP32-S3** e display TFT. O sistema monitora dados vitais do motor em tempo real, oferecendo uma interface moderna e funcional.

## 🚀 Funcionalidades
- **Monitoramento em Tempo Real:** Leitura de sensores (Temperatura, RPM, Tensão da Bateria).
- **Interface Dual:** Temas selecionáveis para visualização Dia e Noite.
- **Gráficos Fluidos:** Ponteiros e mostradores com atualização de alta frequência.
- **Conectividade:** Suporte a Bluetooth Low Energy (BLE) para configuração via app.

## 🛠️ Hardware Utilizado
- **Microcontrolador:** Lolin S3 Mini (ESP32-S3)(pode funcionar com outros também
desde que configurado corretamente).
- **Display:** TFT colorido (SPI).
- **Sensores:** Integração com a rede elétrica e sensores originais do veículo.

## 📸 Demonstração
[cite_start]Aqui você pode inserir as fotos que enviou por e-mail (como a `IMG_9054.JPG` e `IMG_9003.JPG` [cite: 99, 120]):

| Mostrador em Operação | Detalhes da Instalação |
| :---: | :---: |
| ![Mostrador Dia](caminho/para/sua/foto1.jpg) | ![Circuito](caminho/para/sua/foto2.jpg) |

## 🎥 Vídeos do Projeto (YouTube)
Confira o funcionamento do mostrador no painel do carro:
- [ASSISTA AQUI: Demonstração do Mostrador no Uno Mille] https://youtu.be/h0L4GuoAtVM?si=4B_XCHNRT67KqYEl
- [ASSISTA AQUI: Troca de Temas e Interface](LINK_DO_SEU_VIDEO_AQUI)

## 💻 Como Compilar
Este projeto foi desenvolvido utilizando o **PlatformIO** no VS Code.
1. Clone o repositório.
2. Abra a pasta no VS Code com a extensão PlatformIO instalada.
3. Clique em `Build` e depois em `Upload` para o ESP32-S3.

## 📄 Licença
Este projeto está sob a licença MIT.

## Agradecimentos
 Agradeço ao Bodmer pelo excelente trabalho criando a biblioteca TFT_eSPI
 https://github.com/Bodmer/TFT_espi
