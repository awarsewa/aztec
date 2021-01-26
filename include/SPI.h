//
//  SPI.h
//  aztec
//
//  Created by Alexander Warsewa on 31.01.16.
//
//

#ifndef aztec_SPI_h
#define aztec_SPI_h

//returns whether the SPI interrupt flag is set
#define SPItranfserCompleted()  ((SPSR & (1 << SPIF)) ? 1 : 0)
#define SPIbusy()               ((SPSR & (1 << SPIF)) ? 0 : 1)
#define SPIwrite(data)          SPDR = (uint8_t)data

#endif
