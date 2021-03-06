/* DataOutput.java -- Interface for writing data from a stream
   Copyright (C) 1998, 1999, 2001, 2003, 2005  Free Software Foundation, Inc.

This file is part of GNU Classpath.

GNU Classpath is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.
 
GNU Classpath is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Classpath; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */

package java.io;

/**
 * This interface is implemented by classes that can wrte data to streams 
 * from Java primitive types.  This data can subsequently be read back
 * by classes implementing the <code>DataInput</code> interface. 
 *
 * @author Aaron M. Renn (arenn@urbanophile.com)
 * @author Tom Tromey (tromey@cygnus.com)
 *
 * @see DataInput
 */
public interface DataOutput
{
    /**
     * This method writes an 8-bit value (passed into the method as a Java
     * <code>int</code>) to an output stream.  The low 8 bits of the
     * passed value are written.
     *
     * @param value The <code>byte</code> to write to the output stream
     *
     * @throws IOException If an error occurs
     */
    void write(int value) throws IOException;

    /**
     * This method writes the raw byte array passed in to the output stream.
     *
     * @param buf The byte array to write
     *
     * @throws IOException If an error occurs
     */
    void write(byte[] buf) throws IOException;

    /**
     * This method writes raw bytes from the passed array <code>buf</code> 
     * starting
     * <code>offset</code> bytes into the buffer.  The number of bytes 
     * written will be exactly <code>len</code>. 
     *
     * @param buf The buffer from which to write the data
     * @param offset The offset into the buffer to start writing data from
     * @param len The number of bytes to write from the buffer to the output 
     * stream
     *
     * @throws IOException If any other error occurs
     */
    void write(byte[] buf, int offset, int len) throws IOException;

    /**
     * This method writes a Java boolean value to an output stream.  If
     * <code>value</code> is <code>true</code>, a byte with the value of
     * 1 will be written, otherwise a byte with the value of 0 will be 
     * written.
     *
     * The value written can be read using the <code>readBoolean</code>
     * method in <code>DataInput</code>.
     *
     * @param value The boolean value to write
     *
     * @throws IOException If an error occurs
     *
     * @see DataInput#readBoolean
     */
    void writeBoolean(boolean value) throws IOException;

    /**
     * This method writes a Java byte value to an output stream.  The
     * byte to be written will be in the lowest 8 bits of the 
     * <code>int</code> value passed.
     *
     * The value written can be read using the <code>readByte</code> or
     * <code>readUnsignedByte</code> methods in <code>DataInput</code>.
     *
     * @param value The int value to write
     *
     * @throws IOException If an error occurs
     *
     * @see DataInput#readByte
     * @see DataInput#readUnsignedByte
     */
    void writeByte(int value) throws IOException;

    /**
     * This method writes a Java short value to an output stream.  The
     * char to be written will be in the lowest 16 bits of the <code>int</code>
     * value passed.  These bytes will be written "big endian".  That is,
     * with the high byte written first in the following manner:
     * <p>
     * <code>byte0 = (byte)((value & 0xFF00) >> 8);<br>
     * byte1 = (byte)(value & 0x00FF);</code>
     * <p>
     *
     * The value written can be read using the <code>readShort</code> and
     * <code>readUnsignedShort</code> methods in <code>DataInput</code>.
     *
     * @param value The int value to write as a 16-bit value
     *
     * @throws IOException If an error occurs
     *
     * @see DataInput#readShort
     * @see DataInput#readUnsignedShort
     */
    void writeShort(int value) throws IOException;
 
    /**
     * This method writes a Java char value to an output stream.  The
     * char to be written will be in the lowest 16 bits of the <code>int</code>
     * value passed.  These bytes will be written "big endian".  That is,
     * with the high byte written first in the following manner:
     * <p>
     * <code>byte0 = (byte)((value & 0xFF00) >> 8);<br>
     * byte1 = (byte)(value & 0x00FF);</code>
     * <p>
     *
     * The value written can be read using the <code>readChar</code>
     * method in <code>DataInput</code>.
     *
     * @param value The char value to write
     *
     * @throws IOException If an error occurs
     *
     * @see DataInput#readChar
     */
    void writeChar(int value) throws IOException;

    /**
     * This method writes a Java int value to an output stream.  The 4 bytes
     * of the passed value will be written "big endian".  That is, with
     * the high byte written first in the following manner:
     * <p>
     * <code>byte0 = (byte)((value & 0xFF000000) >> 24);<br>
     * byte1 = (byte)((value & 0x00FF0000) >> 16);<br>
     * byte2 = (byte)((value & 0x0000FF00) >> 8);<br>
     * byte3 = (byte)(value & 0x000000FF);</code>
     * <p>
     *
     * The value written can be read using the <code>readInt</code>
     * method in <code>DataInput</code>.
     *
     * @param value The int value to write
     *
     * @throws IOException If an error occurs
     *
     * @see DataInput#readInt
     */
    void writeInt(int value) throws IOException;

    /**
     * This method writes a Java long value to an output stream.  The 8 bytes
     * of the passed value will be written "big endian".  That is, with
     * the high byte written first in the following manner:
     * <p>
     * <code>byte0 = (byte)((value & 0xFF00000000000000L) >> 56);<br>
     * byte1 = (byte)((value & 0x00FF000000000000L) >> 48);<br>
     * byte2 = (byte)((value & 0x0000FF0000000000L) >> 40);<br>
     * byte3 = (byte)((value & 0x000000FF00000000L) >> 32);<br>
     * byte4 = (byte)((value & 0x00000000FF000000L) >> 24);<br>
     * byte5 = (byte)((value & 0x0000000000FF0000L) >> 16);<br>
     * byte6 = (byte)((value & 0x000000000000FF00L) >> 8);<br>
     * byte7 = (byte)(value & 0x00000000000000FFL);</code>
     * <p>
     *
     * The value written can be read using the <code>readLong</code>
     * method in <code>DataInput</code>.
     *
     * @param value The long value to write
     *
     * @throws IOException If an error occurs
     *
     * @see DataInput#readLong
     */
    void writeLong(long value) throws IOException;

    /**
     * This method writes a Java <code>float</code> value to the stream.  This
     * value is written by first calling the method
     * <code>Float.floatToIntBits</code>
     * to retrieve an <code>int</code> representing the floating point number,
     * then writing this <code>int</code> value to the stream exactly the same
     * as the <code>writeInt()</code> method does.
     *
     * The value written can be read using the <code>readFloat</code>
     * method in <code>DataInput</code>.
     *
     * @param value The float value to write
     *
     * @throws IOException If an error occurs
     *
     * @see #writeInt
     * @see DataInput#readFloat
     * @see Float#floatToIntBits
     */
    void writeFloat(float value) throws IOException;

    /**
     * This method writes a Java <code>double</code> value to the stream.  This
     * value is written by first calling the method
     * <code>Double.doubleToLongBits</code>
     * to retrieve an <code>long</code> representing the floating point number,
     * then writing this <code>long</code> value to the stream exactly the same
     * as the <code>writeLong()</code> method does.
     *
     * The value written can be read using the <code>readDouble</code>
     * method in <code>DataInput</code>.
     *
     * @param value The double value to write
     *
     * @throws IOException If any other error occurs
     *
     * @see #writeLong
     * @see DataInput#readDouble
     * @see Double#doubleToLongBits
     */
    void writeDouble(double value) throws IOException;

    /**
     * This method writes a Java <code>String</code> to the stream in a modified
     * UTF-8 format.  First, two bytes are written to the stream indicating the
     * number of bytes to follow.  This is written in the form of a Java
     * <code>short</code> value in the same manner used by the 
     * <code>writeShort</code> method.  Note that this is the number of 
     * bytes in the
     * encoded <code>String</code> not the <code>String</code> length.  Next
     * come the encoded characters.  Each character in the <code>String</code>
     * is encoded as either one, two or three bytes.  For characters in the
     * range of <code>u0001</code> to <code>u007F</code>, one byte is used.  
     * The character
     * value goes into bits 0-7 and bit eight is 0.  For characters in the range
     * of <code>u0080</code> to <code>u007FF</code>, two bytes are used.  Bits
     * 6-10 of the character value are encoded bits 0-4 of the first byte, with
     * the high bytes having a value of "110".  Bits 0-5 of the character value
     * are stored in bits 0-5 of the second byte, with the high bits set to
     * "10".  This type of encoding is also done for the null character
     * <code>u0000</code>.  This eliminates any C style NUL character values
     * in the output.  All remaining characters are stored as three bytes.
     * Bits 12-15 of the character value are stored in bits 0-3 of the first
     * byte.  The high bits of the first bytes are set to "1110".  Bits 6-11
     * of the character value are stored in bits 0-5 of the second byte.  The
     * high bits of the second byte are set to "10".  And bits 0-5 of the
     * character value are stored in bits 0-5 of byte three, with the high bits
     * of that byte set to "10".
     *
     * The value written can be read using the <code>readUTF</code>
     * method in <code>DataInput</code>.
     *
     * @param value The <code>String</code> to write
     *
     * @throws IOException If an error occurs
     *
     * @see DataInput#readUTF
     */
    void writeUTF(String value) throws IOException;
} // interface DataOutput

