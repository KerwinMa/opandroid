package com.openpeer.sample;

import android.app.Activity;
import android.app.Fragment;
import android.os.Bundle;
import com.openpeer.sample.R;

public class BaseFragmentActivity extends Activity{

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_container);
	}
	public Fragment setContentFragment(Fragment fragment){
		this.getFragmentManager().beginTransaction().replace(R.id.fragment_container, fragment).commit();
		return fragment;
	}
}