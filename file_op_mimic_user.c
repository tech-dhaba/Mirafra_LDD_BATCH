// Simulate user interaction with the "device"
 55 int main() {
 56     char read_buffer[10];
 57 
 58     // Mimic opening the device
 59     fops.open();
 60 
 61     // Mimic reading from the device
 62     fops.read(read_buffer, sizeof(read_buffer));
 63     printf("Data read: ");
 64     for (size_t i = 0; i < sizeof(read_buffer); i++) {
 65         putchar(read_buffer[i]);
 66     }
 67     putchar('\n');
 68 
 69     // Mimic writing to the device
 70     fops.write("Hello", 5);
 71 
 72     // Mimic closing the device
 73     fops.release();
 74 
 75     return 0;
 76 }

