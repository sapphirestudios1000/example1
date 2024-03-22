package hla.rti13.java1;

public class NameNotFound extends RTIexception
{
	private static final long serialVersionUID = 98121116105109L;
	
	public NameNotFound( String reason )
	{
		super( reason );
	}

	public NameNotFound( String reason, int serial )
	{
		super( reason, serial );
	}
	
	public NameNotFound()
    {
	    super();
    }

    /**
     * @param cause The cause of the exception
     */
    public NameNotFound( Throwable cause )
    {
	    super( cause );
    }

    /**
     * @param message The message to create the exception with
     * @param cause The cause of the exception
     */
    public NameNotFound( String message, Throwable cause )
    {
	    super( message, cause );
    }
}
