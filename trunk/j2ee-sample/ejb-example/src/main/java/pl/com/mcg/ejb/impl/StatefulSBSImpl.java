package pl.com.mcg.ejb.impl;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collection;
import java.util.Formatter;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;
import javax.annotation.Resource;
import javax.ejb.PostActivate;
import javax.ejb.PrePassivate;
import javax.ejb.Remote;
import javax.ejb.Remove;
import javax.ejb.SessionContext;
import javax.ejb.Stateful;

import pl.com.mcg.ejb.remote.RemoteStatefulSBS;
import pl.com.mcg.ejb.service.SessionBeanService;

@Stateful
@Remote(RemoteStatefulSBS.class)
public class StatefulSBSImpl implements SessionBeanService {
	@Resource
	private SessionContext ctx;

	private ArrayList<String> collection;

	@Override
	public void addItem(String text) {
		collection.add(text);
	}

	@Override
	public Collection<String> getItems() {
		return collection;
	}

	@Override
	public String getTime() {
		System.out.println(ctx.getInvokedBusinessInterface());

		Formatter fmt = new Formatter();
		Calendar cal = Calendar.getInstance();
		fmt.format("%tr", cal);
		return fmt.toString();
	}

	@PrePassivate
	public void logPassivation() {
		System.out.println("PrePassivate method logPassivation() Invoked");
	}

	@PostActivate
	public void logActivation() {
		System.out.println("PostActivate method logActivation() Invoked");
	}

	@Remove
	public void finished() {
		System.out.println("Remove method finished() Invoked");
	}

	@PostConstruct
	public void init() {
		System.out.println("Post Constructor Method init() Invoked");
		collection = new ArrayList<String>();
	}

	@PreDestroy
	public void tidyUp() {
		System.out.println("Pre Destruction Method tidyUp() Invoked");
		collection.clear();
	}
}