package secretpin;
import javacard.framework.APDU;
import javacard.framework.Applet;
import javacard.framework.ISO7816;
import javacard.framework.ISOException;
import secretpin.OwnerPin;
import javacard.framework.Util;
import javacardx.apdu.ExtendedLength;
/**
 * Top Secret Applet for accessing a specific building requiring a password 
 * Any employee getting robbed for the card is responsible !!!
 * 
 * Heavily protected thanks to an OwnerPin class, so nothing bad can happen
 * 
 */
public class SecretPin extends Applet implements ExtendedLength{

    /* constants declaration */
    final static byte IDApplet_CLA = (byte) 0x80;
    
    final static byte VERIFY = (byte) 0x20;
    final static byte GET_FLAG = (byte) 0x50;
    final static byte WRITE_FLAG = (byte) 0x30;
    
    final static short FLAG_SIZE = (short) 0x0F35;
    
    final static short SW_VERIFICATION_FAILED = 0x6300;
    
    
    final static byte SECRET_PIN_SIZE = ??; // REDACTED !! 3:) 
    final static byte[] SECRET_PIN = { ?? }; // REDACTED !! 3:) 
    byte[] flag;

    /* instance variables declaration */
    OwnerPin pin;
    
    private SecretPin(byte[] bArray, short bOffset, byte bLength) {
        pin = new OwnerPin(SECRET_PIN, SECRET_PIN_SIZE);
        flag = new byte[FLAG_SIZE];
        register();

    } // end of the constructor

    public static void install(byte[] bArray, short bOffset, byte bLength) {
        new SecretPin(bArray, bOffset, bLength);
    } // end of install method

    @Override
    public boolean select() {
        // No more tries
        if (pin.getTriesRemaining() == (byte) 0x0) {
            return false;
        }

        return true;

    }// end of select method

    public boolean is_pin_valid(APDU apdu) {
        /**
         * Method for checking if the pin has been validated already
         * Returns true if the pin has been validated
         */
        
        boolean ret = pin.isValidated();
        return ret;
    }
    
    @Override
    public void process(APDU apdu) {

        byte[] buffer = apdu.getBuffer();
        
        if (apdu.isISOInterindustryCLA()) {
            if (buffer[ISO7816.OFFSET_INS] == (byte) (0xA4)) {
                return;
            }
            ISOException.throwIt(ISO7816.SW_CLA_NOT_SUPPORTED);
        }

        if (buffer[ISO7816.OFFSET_CLA] != IDApplet_CLA) {
            ISOException.throwIt(ISO7816.SW_CLA_NOT_SUPPORTED);
        }
        
        switch (buffer[ISO7816.OFFSET_INS]) {
            case GET_FLAG:
                if(is_pin_valid(apdu))
                    getFlag(apdu);
                return;
            case VERIFY:
                // First, we need to check the PIN code to ensure the user is allowed
                verify(apdu);
                return;
            case WRITE_FLAG:
                writeSecretData(apdu);
                return;
            default:
                ISOException.throwIt(ISO7816.SW_INS_NOT_SUPPORTED);
        }

    }
    
    private void writeSecretData(APDU apdu) {
        byte[] buffer = apdu.getBuffer();
        Util.arrayCopy(buffer, (byte) 0x07, this.flag, (byte) 0x00, FLAG_SIZE);
        apdu.setOutgoingAndSend((byte) 0x02, FLAG_SIZE);
        
    }
    
    private void getFlag(APDU apdu) {
        byte[] buffer = apdu.getBuffer();
        Util.arrayCopy(this.flag, (short) 0x00, buffer, (short) 0x00, FLAG_SIZE);
        apdu.setOutgoingAndSend((short) 0x00, FLAG_SIZE);
    }
    
    private void getPinCode(APDU apdu) {
        byte[] buffer = apdu.getBuffer();
        Util.arrayCopy(this.pin.pin_code, (short) 0x00, buffer, (short) 0x00, SECRET_PIN_SIZE);
        apdu.setOutgoingAndSend((short) 0x00, SECRET_PIN_SIZE);
    }

    private void verify(APDU apdu) {
        byte[] buffer = apdu.getBuffer();
        // retrieve the PIN data for validation.
        if (pin.check(buffer, ISO7816.OFFSET_CDATA, SECRET_PIN_SIZE) == false) {
            ISOException.throwIt(SW_VERIFICATION_FAILED);
        }

    }

} // end of class