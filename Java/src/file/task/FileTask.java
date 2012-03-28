package file.task;

import java.io.IOException;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.EnumSet;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author uko
 */
public class FileTask
{
	public static Path cpdirtree(final Path source, final Path target) throws IOException
	{
		if(source.toString().equals("")) throw new IOException("Source path is empty.");		
		if(target.toString().equals("")) throw new IOException("Target path is empty.");		
		if(!Files.exists(source)) throw new IOException("Can not find source: " + source);
		if(!Files.isDirectory(source)) throw new IOException("Source isn't a directory: " + source);
		if(!Files.isReadable(source)) throw new IOException("Insufficient privileges of source: " + source);		
		if(!Files.isReadable(source)) throw new IOException("Insufficient privileges of source: " + source);		
		if(!Files.exists(target)) throw new IOException("Can not find target: " + target);
		if(!Files.isReadable(target)) throw new IOException("Insufficient privileges of target: " + source);
		
		Files.walkFileTree(source, EnumSet.noneOf(FileVisitOption.class), 2, new SimpleFileVisitor<Path>()
		{
			@Override
			public FileVisitResult preVisitDirectory (Path dir, BasicFileAttributes attrs) throws IOException
			{
				Path targetdir = target.resolve(source.relativize(dir));
				if(!Files.exists(targetdir))
				{
					try
					{
						Files.createDirectory(targetdir);
						Files.walkFileTree(dir, new SimpleFileVisitor<Path>()
						{
							@Override
							public FileVisitResult preVisitDirectory(Path dir, BasicFileAttributes attrs) throws IOException
							{
								Path targetdir = target.resolve(source.relativize(dir));
								try
								{
									Files.createDirectory(targetdir);
								} catch (FileAlreadyExistsException e)
								{
									if (!Files.isDirectory(targetdir))
									{
										throw e;
									}
								}
								return FileVisitResult.CONTINUE;
							}
							@Override
							public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException
							{
								Files.copy(file, target.resolve(source.relativize(file)));
								return FileVisitResult.CONTINUE;
							}
						});
					}
					catch (FileAlreadyExistsException e)
					{
						if (!Files.isDirectory(targetdir))
						{
							throw e;
						}
					}
				}
				return FileVisitResult.CONTINUE;
			}
		});
				
		return target;
	}

	
	/**
	 * @param args the command line arguments
	 */
	public static void main(String[] args)
	{
		String usage = "Usage: SRC DST\n"
				+ "\tSRC    path to directory to copy from\n"
				+ "\tDST    path to directory to copy to\n";
		Path src;
		Path dst;
		// Ensure necessary arguments passed.
		if (args.length < 2)
		{
			System.err.println(usage);
			System.exit(0);
		}

		src = FileSystems.getDefault().getPath(args[0]);
		dst = FileSystems.getDefault().getPath(args[1]);
		try
		{
			cpdirtree(src, dst);
			System.out.println("Copying done.");
		} catch (IOException ex)
		{
			Logger.getLogger(FileTask.class.getName()).log(Level.SEVERE, null, ex);
		}
	}
}
