cd build
cd utility
touch test2.jpg
echo "Compressing png"
./huffman -c /home/max/Рабочий\ стол/huffman_encoder/test.jpg /home/max/Рабочий\ стол/huffman_encoder/dst.huf
./huffman -d /home/max/Рабочий\ стол/huffman_encoder/dst.huf /home/max/Рабочий\ стол/huffman_encoder/test2.jpg
